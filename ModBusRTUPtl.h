/*
 * ModBusRTUPtl.h
 *
 *  Created on: 2020年1月16日
 *      Author: Bill Huang
 */

#ifndef SRC_MODBUSRTUPTL_H_
#define SRC_MODBUSRTUPTL_H_


typedef unsigned char BYTE;


typedef enum RegValueFormat
{
	UnSignedShort = 0, //寄存器值为无符号短整型，高字节在前低字节在后
	SignedShort   = 1, //寄存器值为有符号短整型，高字节在前低字节在后
	Float4321     = 2, //寄存器值为单精度浮点数，两个03或04寄存器存放一个浮点数，字节顺序为接收缓存第一字节为浮点数的第4字节，缓存的第二字节为浮点数第3字节，缓存的第三字节为浮点数第2字节，缓存的第四字节为浮点数的第1字节
	Float2143     = 3, //寄存器值为单精度浮点数，两个03或04寄存器存放一个浮点数，字节顺序为接收缓存第一字节为浮点数的第2字节，缓存的第二字节为浮点数第1字节，缓存的第三字节为浮点数第4字节，缓存的第四字节为浮点数的第3字节
	Float1234     = 4, //寄存器值为单精度浮点数，两个03或04寄存器存放一个浮点数，字节顺序为接收缓存第一字节为浮点数的第1字节，缓存的第二字节为浮点数第2字节，缓存的第三字节为浮点数第3字节，缓存的第四字节为浮点数的第4字节
	Float3412     = 5, //寄存器值为单精度浮点数，两个03或04寄存器存放一个浮点数，字节顺序为接收缓存第一字节为浮点数的第3字节，缓存的第二字节为浮点数第4字节，缓存的第三字节为浮点数第1字节，缓存的第四字节为浮点数的第2字节
	Double8_1     = 6, //寄存器值为双精度浮点数，四个03或04寄存器存放一个浮点数，高字节在前低字节在后
	Double1_8     = 7  //寄存器值为双精度浮点数，四个03或04寄存器存放一个浮点数，低字节在前高字节在后
}RegFormat;

typedef enum ModbusRetCode
{
	M_NoResponse = -5,
	M_CRCError = -4,
	M_UnPacketErr = -3,
	M_PacketErr = -2,
	M_WriteComErr = -1,
	M_SUCCESS = 0,
	M_ILLEGAL_FUNCTION = 1,
	M_ILLEGAL_DATA_ADDRESS = 2,
	M_ILLEGAL_DATA_VALUE = 3,
	M_SLAVE_DEVICE_FAILURE = 4
}ModRetCode;

/** 
 * @brief ModBus-RTU协议中的0x01,0x02,0x03,0x04功能号发送数据帧打包函数
 * @param[in] funcCode    功能号：01、02、03、04
 * @param[in] slaveAdd    从站地址
 * @param[in] startReg	  开始寄存器地址
 * @param[in] regNums	  寄存器个数
 * @param[out] buffer	  返回的数据帧发送缓存
 * @param[out] recvBytes  此发送帧发送后从站应该返回的正常接收帧的字节个数
 *     -<em>大于0</em> 发送数据帧长度
 *     -<em>等于0</em> 打包失败，错误的传入参数
 **/
int Fun01020304Pacet(BYTE funcCode, unsigned short slaveAdd,unsigned short startReg, unsigned short regNums,BYTE* buffer, unsigned short* recvBytes);

/** 
 * @brief ModBus-RTU协议中0x01读取线圈寄存器功能号数据帧解包函数
 * @param[in] buffer      接收数据缓存区
 * @param[in] bufferLen   接收到的字节数
 * @param[in] regNums	  发送读取数据帧时的寄存器个数，用于校验是否是此请求返回的数据帧
 * @param[out] slaveAdd	  返回接收数据帧中的从站地址
 * @param[out] regValues   解包得到的寄存器值
 * @return 返回说明
 *     -<em>0</em> succeed
 *     -<em>!0</em> fail
 **/
int Fun01UnPacket(const BYTE *buffer, unsigned short bufferLen, unsigned short regNums, unsigned short *slaveAdd, unsigned short* regValues);

/** 
 * @brief ModBus-RTU协议中0x02读取数字量状态寄存器功能号数据帧解包函数
 * @param[in] buffer      接收数据缓存区
 * @param[in] bufferLen   接收到的字节数
 * @param[in] regNums	  发送读取数据帧时的寄存器个数，用于校验是否是此请求返回的数据帧
 * @param[out] slaveAdd	  返回接收数据帧中的从站地址
 * @param[out] regValues   解包得到的寄存器值
 * @return 返回说明
 *     -<em>0</em> succeed
 *     -<em>!0</em> fail
 **/
int Fun02UnPacket(const BYTE *buffer, unsigned short bufferLen, unsigned short regNums, unsigned short *slaveAdd, unsigned short* regValues);


/** 
 * @brief ModBus-RTU协议中0x03读取保持寄存器功能号数据帧解包函数
 * @param[in] buffer      接收数据缓存区
 * @param[in] bufferLen   接收到的字节数
 * @param[in] regNums	  发送读取数据帧时的寄存器个数，用于校验是否是此请求返回的数据帧
 * @param[out] slaveAdd	  返回接收数据帧中的从站地址
 * @param[out] regValues   解包得到的寄存器值
 * @param[in] regFormat   寄存器存放数据的格式（参考RegFormat枚举）
 * @return 返回说明
 *     -<em>0</em> succeed
 *     -<em>!0</em> fail
 **/
int Fun03UnPacket(const BYTE *buffer, unsigned short bufferLen, unsigned short regNums, unsigned short *slaveAdd, double* regValues, RegFormat regFormat);

/** 
 * @brief ModBus-RTU协议中0x04读取输入寄存器功能号数据帧解包函数
 * @param[in] buffer      接收数据缓存区
 * @param[in] bufferLen   接收到的字节数
 * @param[in] regNums	  发送读取数据帧时的寄存器个数，用于校验是否是此请求返回的数据帧
 * @param[out] slaveAdd	  返回接收数据帧中的从站地址
 * @param[out] regValues   解包得到的寄存器值
 * @param[in] regFormat   寄存器存放数据的格式（参考RegFormat枚举）
 * @return 返回说明
 *     -<em>0</em> succeed
 *     -<em>!0</em> fail
 **/
int Fun04UnPacket(const BYTE *buffer, unsigned short bufferLen, unsigned short regNums, unsigned short *slaveAdd, double* regValues, RegFormat regFormat);

/** 
 * @brief ModBus-RTU协议中的0x05功能号写入单个线圈寄存器命令数据帧打包函数
 * @param[in] slaveAdd    从站地址
 * @param[in] startReg	  线圈寄存器地址
 * @param[in] openOrclose	  写入线圈寄存器的值，非0:闭合，0：断开
 * @param[out] buffer	  返回的命令数据帧缓冲区
 * @return 返回说明
 *     -<em>大于0</em> 数据帧长度 
 *     -<em>等于0</em> 打包失败，错误的传入参数
 **/
int Fun05Pack(unsigned short slaveAdd, unsigned short startReg, BYTE openOrclose, BYTE* buffer);

/** 
 * @brief ModBus-RTU协议中0x05功能号写入单个线圈寄存器从站返回数据帧解包函数
 * @param[in] buffer      接收数据缓存区
 * @param[in] bufferLen   接收到的字节数
 * @param[out] slaveAdd	  返回接收数据帧中的从站地址
 * @param[in] regAdd	  写入的寄存器地址，用于校验是否是此请求返回的数据帧
 * @param[out] regValues   解包得到的命令发送帧写入的寄存器值
 * @return 返回说明
 *     -<em>0</em> succeed
 *     -<em>!0</em> fail
 **/
int Fun05UnPack(const BYTE* buffer, unsigned short bufferLen, unsigned short *slaveAdd, unsigned short *regAdd, BYTE* regValue);

/** 
 * @brief ModBus-RTU协议中的0x0F功能号写入多个线圈寄存器命令数据帧打包函数
 * @param[in] slaveAdd    从站地址
 * @param[in] startReg	  线圈寄存器开始地址
 * @param[in] regNums	  线圈寄存器个数
 * @param[in] regValues	  写入线圈寄存器的值数组，非0:闭合，0：断开
 * @param[out] buffer	  返回的命令数据帧缓冲区
 * @return 返回说明
 *     -<em>大于0</em> 数据帧长度 
 *     -<em>等于0</em> 打包失败，错误的传入参数
 **/
int Fun0FPack(unsigned short slaveAdd, unsigned short startReg, unsigned short regNums, const BYTE* regValues, BYTE* buffer);

/** 
 * @brief ModBus-RTU协议中0x0F功能号写入多个线圈寄存器从站返回数据帧解包函数
 * @param[in] buffer      接收数据缓存区
 * @param[in] bufferLen   接收到的字节数
 * @param[out] slaveAdd	  返回接收数据帧中的从站地址
 * @param[out] startReg	  返回的数据帧中的开始寄存器地址
 * @param[out] regNums    返回的数据帧中的寄存器个数
 * @return 返回说明
 *     -<em>0</em> succeed
 *     -<em>!0</em> fail
 **/
int Fun0FUnPacket(const BYTE* buffer, int bufferLen, unsigned short* slaveAdd, unsigned short* startReg, unsigned short* regNums);

/** 
 * @brief ModBus-RTU协议中的0x10功能号写入多个保持寄存器命令数据帧打包函数
 * @param[in] slaveAdd    从站地址
 * @param[in] startReg	  寄存器开始地址
 * @param[in] regNums	  寄存器个数
 * @param[in] regValues	  写入寄存器的值数组
 * @param[out] buffer	  返回的命令数据帧缓冲区
 * @param[in] regFormat	  regValues寄存器值按何种数值格式写入寄存器
 * @return 返回说明
 *     -<em>大于0</em> 数据帧长度 
 *     -<em>等于0</em> 打包失败，错误的传入参数
 **/
int Fun10Packet(unsigned short slaveAdd, unsigned short startReg, unsigned short regNums, const double* regValues, BYTE* buffer, RegFormat regFormat);

/** 
 * @brief ModBus-RTU协议中0x10功能号写入多个保持寄存器从站返回数据帧解包函数
 * @param[in] buffer      接收数据缓存区
 * @param[in] bufferLen   接收到的字节数
 * @param[out] slaveAdd	  返回接收数据帧中的从站地址
 * @param[out] startReg	  返回的数据帧中的开始寄存器地址
 * @param[out] regNums    返回的数据帧中的寄存器个数
 * @return 返回说明
 *     -<em>0</em> succeed
 *     -<em>!0</em> fail
 **/
int Fun10UnPacket(const BYTE* buffer, int bufferLen, unsigned short* slaveAdd, unsigned short* startReg, unsigned short* regNums);



#endif /* SRC_MODBUSRTUPTL_H_ */
