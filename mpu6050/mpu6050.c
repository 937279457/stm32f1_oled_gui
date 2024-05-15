#include "mpu6050.h"

#define DEFAULT_MPU_HZ  (100)
#define Q30 			(1073741824.0f)

static MYI2C_InitTypeDef* p;

static signed char gyro_orientation[9] = {-1, 0, 0,
                                           0,-1, 0,
                                           0, 0, 1};

/* These next two functions converts the orientation matrix (see
 * gyro_orientation) to a scalar representation for use by the DMP.
 * NOTE: These functions are borrowed from Invensense's MPL.
 */
static unsigned short inv_row_2_scale(const signed char *row)
{
    unsigned short b;

    if (row[0] > 0)
        b = 0;
    else if (row[0] < 0)
        b = 4;
    else if (row[1] > 0)
        b = 1;
    else if (row[1] < 0)
        b = 5;
    else if (row[2] > 0)
        b = 2;
    else if (row[2] < 0)
        b = 6;
    else
        b = 7;      // error
    return b;
}

static unsigned short inv_orientation_matrix_to_scalar(
    const signed char *mtx)
{
    unsigned short scalar;

    /*
       XYZ  010_001_000 Identity Matrix
       XZY  001_010_000
       YXZ  010_000_001
       YZX  000_010_001
       ZXY  001_000_010
       ZYX  000_001_010
     */

    scalar = inv_row_2_scale(mtx);
    scalar |= inv_row_2_scale(mtx + 3) << 3;
    scalar |= inv_row_2_scale(mtx + 6) << 6;


    return scalar;
}

static int run_self_test(void)
{
    int result;
    long gyro[3], accel[3];

    result = mpu_run_self_test(gyro, accel);
    if (result == 0x3) {
        /* Test passed. We can trust the gyro data here, so let's push it down
         * to the DMP.
         */
        float sens;
        unsigned short accel_sens;
        mpu_get_gyro_sens(&sens);
        gyro[0] = (long)(gyro[0] * sens);
        gyro[1] = (long)(gyro[1] * sens);
        gyro[2] = (long)(gyro[2] * sens);
        dmp_set_gyro_bias(gyro);
        mpu_get_accel_sens(&accel_sens);
        accel[0] *= accel_sens;
        accel[1] *= accel_sens;
        accel[2] *= accel_sens;
        dmp_set_accel_bias(accel);
    }
    else
    {
        return -1;
    }
    return 0;
}

static int MPU6050_DMPInit(MYI2C_InitTypeDef* pMYI2C_InitStruct)
{
    mpu6050_DMP_I2C_Init(pMYI2C_InitStruct);
    int result = 0;
    struct int_param_s int_param;
    result = mpu_init(&int_param);
    if(result)
        return -1;
    result = mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL);
    if(result)
        return -2;
    result = mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL);
    if(result)
        return -3;
    result = mpu_set_sample_rate(DEFAULT_MPU_HZ);
    if(result)
        return -4;
    result = dmp_load_motion_driver_firmware();
    if(result)
        return -5;
    result = dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation));
    if(result)
        return -6;
    result = dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |
                                DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | 
                                DMP_FEATURE_SEND_CAL_GYRO | DMP_FEATURE_GYRO_CAL);
    if(result)
        return -7;
    result = dmp_set_fifo_rate(DEFAULT_MPU_HZ);
    if(result)
        return -8;
    result = run_self_test();
    if(result)
        return -9;
    result = mpu_set_dmp_state(1);
    if(result)
        return -10;
    return 0;
}

int MPU6050_DMP_GetCapeEuler(MPU6050_CapeEuler_t* cape_euler)
{
    float q0 = 0.0f;
    float q1 = 0.0f;
    float q2 = 0.0f;
    float q3 = 0.0f;
    short gyro[3];
    short accel[3];
    long quat[4];
    unsigned long timestamp;
    short sensors;
    unsigned char more;
    if(dmp_read_fifo(gyro, accel, quat, &timestamp, &sensors, &more))
    {
        return -1;
    }
    if(sensors & INV_WXYZ_QUAT)
    {
        q0 = quat[0] / Q30;
        q1 = quat[1] / Q30;
        q2 = quat[2] / Q30;
        q3 = quat[3] / Q30;
        
        cape_euler->pitch = asin(-2 * q1 * q3 + 2 * q0 * q2) * 57.3;
        cape_euler->roll = atan2(2 * q2 * q3 + 2 * q0 *q1, -2 * q1 * q1 - 2 * q2 * q2 + 1) * 57.3;
        cape_euler->yaw = atan2(2 * (q1 * q2 +q0 * q3), q0 * q0 +q1 * q1 - q2 * q2 - q3 * q3) * 57.3;
    }
    
    return 0;
}

static void MPU6050_Init(MYI2C_InitTypeDef* pMYI2C_InitStruct)
{
    MYI2C_WriteCmd(p, MPU6050_ADDRESS, MPU6050_PWR_MGMT_1, 0x01);		// 电源管理寄存器1，取消睡眠模式，选择时钟源为X轴陀螺仪
    MYI2C_WriteCmd(p, MPU6050_ADDRESS, MPU6050_PWR_MGMT_2, 0x00);		// 电源管理寄存器2，保持默认值0，所有轴均不待机
    MYI2C_WriteCmd(p, MPU6050_ADDRESS, MPU6050_SMPLRT_DIV, 0x09);		// 采样率分频寄存器，配置采样率
    MYI2C_WriteCmd(p, MPU6050_ADDRESS, MPU6050_CONFIG, 0x06);			// 配置寄存器，配置DLPF,设置滤波器带宽   260Hz:0 186Hz:1 94Hz:2 44Hz:3 21Hz:4 10Hz:5 5Hz:6
    MYI2C_WriteCmd(p, MPU6050_ADDRESS, MPU6050_GYRO_CONFIG, 0x08);	    // 陀螺仪配置寄存器不自检，选择量程      ±250°/s:0x00 ±500°/s:0x08 ±1000°/s:0x10 ±2000°/s:0x18
    MYI2C_WriteCmd(p, MPU6050_ADDRESS, MPU6050_ACCEL_CONFIG, 0x08);	    // 加速度计配置寄存器不自检，选择量程    ±2g    :0x00 ±4g    :0x08 ±8g     :0x10 ±16g    :0x18
    // 量程更改后需要更改量程宏ACCRANGE和GRYORANGE
}

int MPU6050_SoftwareInit(GPIO_TypeDef* GPIOx, uint16_t SCL, uint16_t SDA, FunctionalState DMPCmd)
{
    static MYI2C_InitTypeDef MYI2C_InitStructure;
    MYI2C_InitStructure.MYI2C_Mode = MYI2C_Mode_Software;
    MYI2C_InitStructure.MYI2C_Pin_SCL = SCL;
    MYI2C_InitStructure.MYI2C_Pin_SDA = SDA;
    MYI2C_InitStructure.MYI2C_GPIOx = GPIOx;
    MYI2C_Init(&MYI2C_InitStructure);
    
    p = &MYI2C_InitStructure;
    
    if(DMPCmd == ENABLE)
        return MPU6050_DMPInit(p);
    else
        MPU6050_Init(p);
    return 0;
}

int MPU6050_I2CInit(I2C_TypeDef* I2Cx, FunctionalState DMPCmd)
{
    static MYI2C_InitTypeDef MYI2C_InitStructure;
    MYI2C_InitStructure.MYI2C_I2Cx = I2Cx;
    MYI2C_InitStructure.MYI2C_Mode = MYI2C_Mode_Hardware;
    MYI2C_InitStructure.MYI2C_ClockSpeed = 50000;   // 为了保持数据稳定，不宜将时钟调的过高
    MYI2C_Init(&MYI2C_InitStructure);
    
    p = &MYI2C_InitStructure;
    
    if(DMPCmd == ENABLE)
        return MPU6050_DMPInit(p);
    else
        MPU6050_Init(p);
    return 0;
}

void MPU6050_GetRawData(MPU6050_RawData_t* data)					
{
    uint8_t DataH, DataL;								                //定义数据高8位和低8位的变量
    
    MYI2C_ReadData(p, MPU6050_ADDRESS, MPU6050_ACCEL_XOUT_H);
    DataH = MYI2C_ReadData(p, MPU6050_ADDRESS, MPU6050_ACCEL_XOUT_H);	//读取加速度计X轴的高8位数据
    DataL = MYI2C_ReadData(p, MPU6050_ADDRESS, MPU6050_ACCEL_XOUT_L);	//读取加速度计X轴的低8位数据
    data->AccX = (DataH << 8) | DataL;						            //数据拼接，通过输出参数返回
    
    DataH = MYI2C_ReadData(p, MPU6050_ADDRESS, MPU6050_ACCEL_YOUT_H);	//读取加速度计Y轴的高8位数据
    DataL = MYI2C_ReadData(p, MPU6050_ADDRESS, MPU6050_ACCEL_YOUT_L);	//读取加速度计Y轴的低8位数据
    data->AccY = (DataH << 8) | DataL;						            //数据拼接，通过输出参数返回
    
    DataH = MYI2C_ReadData(p, MPU6050_ADDRESS, MPU6050_ACCEL_ZOUT_H);	//读取加速度计Z轴的高8位数据
    DataL = MYI2C_ReadData(p, MPU6050_ADDRESS, MPU6050_ACCEL_ZOUT_L);	//读取加速度计Z轴的低8位数据
    data->AccZ = (DataH << 8) | DataL;						            //数据拼接，通过输出参数返回
    
    DataH = MYI2C_ReadData(p, MPU6050_ADDRESS, MPU6050_GYRO_XOUT_H);	//读取陀螺仪X轴的高8位数据
    DataL = MYI2C_ReadData(p, MPU6050_ADDRESS, MPU6050_GYRO_XOUT_L);	//读取陀螺仪X轴的低8位数据
    data->GyroX = (DataH << 8) | DataL;						            //数据拼接，通过输出参数返回
    
    DataH = MYI2C_ReadData(p, MPU6050_ADDRESS, MPU6050_GYRO_YOUT_H);	//读取陀螺仪Y轴的高8位数据
    DataL = MYI2C_ReadData(p, MPU6050_ADDRESS, MPU6050_GYRO_YOUT_L);	//读取陀螺仪Y轴的低8位数据
    data->GyroY = (DataH << 8) | DataL;						            //数据拼接，通过输出参数返回
    
    DataH = MYI2C_ReadData(p, MPU6050_ADDRESS, MPU6050_GYRO_ZOUT_H);	//读取陀螺仪Z轴的高8位数据
    DataL = MYI2C_ReadData(p, MPU6050_ADDRESS, MPU6050_GYRO_ZOUT_L);	//读取陀螺仪Z轴的低8位数据
    data->GyroZ = (DataH << 8) | DataL;						            //数据拼接，通过输出参数返回 
}

void MPU6050_GetData(MPU6050_Data_t* data)
{
    MPU6050_RawData_t raw;
    MPU6050_GetRawData(&raw);
    data->AccX = ((float)raw.AccX / 32767) * ACCRANGE;
    data->AccY = ((float)raw.AccY / 32767) * ACCRANGE;
    data->AccZ = ((float)raw.AccZ / 32767) * ACCRANGE;
    data->GyroX = ((float)raw.GyroX / 32767) * GRYORANGE;
    data->GyroY = ((float)raw.GyroY / 32767) * GRYORANGE;
    data->GyroZ = ((float)raw.GyroZ / 32767) * GRYORANGE;
}

int MPU6050_DetectShake(MPU6050_Direction_e direction, MPU6050_Data_t* data, float threshold)
{
    static uint8_t flag_X_P = 0;
    static uint8_t flag_X_N = 0;
    static uint8_t flag_Y_P = 0;
    static uint8_t flag_Y_N = 0;
    static uint8_t flag_Z_P = 0;
    static uint8_t flag_Z_N = 0;
    uint8_t* p;
    float data_detect = 0;
    switch(direction)
    {
        case X_P:data_detect = data->AccX; p = &flag_X_P; break;
        case X_N:data_detect = -data->AccX; p = &flag_X_N; break;
        case Y_P:data_detect = data->AccY; p = &flag_Y_P; break;
        case Y_N:data_detect = -data->AccY; p = &flag_Y_N; break;
        case Z_P:data_detect = data->AccZ; p = &flag_Z_P; break;
        case Z_N:data_detect = -data->AccZ; p = &flag_Z_N; break;
    }
    if(*p == 0)
    {
        if(data_detect > threshold)
        {
            *p = 1;
            return 1;
        }
    }
    else
    {
        if(data_detect < (threshold - 0.1))
            *p = 0;
    }
    return 0;
}

int MPU6050_DetectDirection(MPU6050_Direction_e direction, MPU6050_Data_t* data, float threshold1, float threshold2)
{
    float data1 = 0;
    float data2 = 0;
    switch(direction)
    {
        case X_P:data1 = data->AccX; data2 = data->AccY; break;
        case X_N:data1 = -data->AccX; data2 = data->AccY; break;
        case Y_P:data1 = data->AccY; data2 = data->AccX; break;
        case Y_N:data1 = -data->AccY; data2 = data->AccX; break;
        default:break;
    }
    if(data2 < threshold2 && data2 > -threshold2)
    {
        if(data1 > threshold1)
            return 1;
    }
    return 0;
}

int MPU6050_DetectRotation(MPU6050_Direction_e direction, MPU6050_Data_t* data, float threshold)
{
    float data_detect = 0;
    switch(direction)
    {
        case X_P: data_detect = data->GyroX; break;
        case X_N: data_detect = -data->GyroX; break;
        case Y_P: data_detect = data->GyroY; break;
        case Y_N: data_detect = -data->GyroY; break;
        case Z_P: data_detect = data->GyroZ; break;
        case Z_N: data_detect = -data->GyroZ; break;
        default:break;
    }
    if(data_detect > threshold)
    {
        return 1;
    }
    return 0;
}

int MPU6050_DetectUp(MPU6050_Data_t* data, float threshold_x, float threshold_y)
{
    if(data->AccY < threshold_y && data->AccY > -threshold_y)
    {
        if(data->AccX > threshold_x)
            return 1;
    }
    return 0;
}

int MPU6050_DetectDown(MPU6050_Data_t* data, float threshold_x, float threshold_y)
{
    if(data->AccY < threshold_y && data->AccY > -threshold_y)
    {
        if(data->AccX < -threshold_x)
            return 1;
    }
    return 0;
}

int MPU6050_DetectRight(MPU6050_Data_t* data, float threshold_y, float threshold_x)
{
    if(data->AccX < threshold_x && data->AccX > -threshold_x)
    {
        if(data->AccY > threshold_y)
            return 1;
    }
    return 0;
}

int MPU6050_DetectLeft(MPU6050_Data_t* data, float threshold_y, float threshold_x)
{
    if(data->AccX < threshold_x && data->AccX > -threshold_x)
    {
        if(data->AccY < -threshold_y)
            return 1;
    }
    return 0;
}

int MPU6050_DetectUpShake(MPU6050_Data_t* data, float threshold_x, float threshold_y)
{
    static uint8_t flag = 0;
    if(data->AccY < threshold_y && data->AccY > -threshold_y)
    {
        if(flag == 0)
        {
            if(data->AccX > threshold_x)
            {
                flag = 1;
                return 1;
            }
        }
        else
        {
            if(data->AccX < (threshold_x - 0.1))
                flag = 0;
        }
    }
    return 0;
}

int MPU6050_DetectDownShake(MPU6050_Data_t* data, float threshold_x, float threshold_y)
{
    static uint8_t flag = 0;
    if(data->AccY < threshold_y && data->AccY > -threshold_y)
    {
        if(flag == 0)
        {
            if(data->AccX < -threshold_x)
            {
                flag = 1;
                return 1;
            }
        }
        else
        {
            if(data->AccX > -(threshold_x  - 0.1))
                flag = 0;
        }
    }
    return 0;
}

int MPU6050_DetectRightShake(MPU6050_Data_t* data, float threshold_y, float threshold_x)
{
    static uint8_t flag = 0;
    if(data->AccX < threshold_x && data->AccX > -threshold_x)
    {
        if(flag == 0)
        {
            if(data->AccY > threshold_y)
            {
                flag = 1;
                return 1;
            }
        }
        else
        {
            if(data->AccY < (threshold_y - 0.1))
                flag = 0;
        }
    }
    return 0;
}

int MPU6050_DetectLeftShake(MPU6050_Data_t* data, float threshold_y, float threshold_x)
{
    static uint8_t flag = 0;
    if(data->AccX < threshold_x && data->AccX > -threshold_x)
    {
        if(flag == 0)
        {
            if(data->AccY < -threshold_y)
            {
                flag = 1;
                return 1;
            }
        }
        else
        {
            if(data->AccY > -(threshold_y - 0.1))
                flag = 0;
        }
    }
    return 0;
}
