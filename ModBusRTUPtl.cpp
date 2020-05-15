/*
 * ModBusRTUPtl.cpp
 *
 *  Created on: 2020年1月16日
 *      Author: Bill Huang
 */
#include "ModBusRTUPtl.h"
#include <string>
#include <cstdio>
#include <cmath>
using namespace std;

static BYTE auchCRCHi[] = {
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40
};

static BYTE auchCRCLo[] = {
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
0x40
};


BYTE LOBYTE(unsigned short wValue)
{
	BYTE byteValue = wValue & 0x00FF;
	return byteValue;
}

BYTE HIBYTE(unsigned short wValue)
{
	BYTE byteValue = wValue >> 8;
	return byteValue;
}

/// <summary>
///     CRC16位校验函数
/// </summary>
/// <param name="puchMsg" type=BYTE[]">
///     <para>
///         要校验的数据缓冲区
///     </para>
/// </param>
/// <param name="usDataLen" type="unsigned short">
///     <para>
///         要校验的数据在缓冲区中的长度
///     </para>
/// </param>
/// <returns>
///     A ushort value...
/// </returns>
unsigned short CRC16(const BYTE* puchMsg, unsigned short usDataLen)
{
	BYTE uchCRCHi = 0xFF; /* high byte of CRC initialized */
	BYTE uchCRCLo = 0xFF; /* low byte of CRC initialized */
	unsigned uIndex; /* will index into CRC lookup table */
	int nIndex = 0;

	while (usDataLen--) /* pass through message buffer */
	{
		uIndex = uchCRCHi ^ puchMsg[nIndex++]; /* calculate the CRC */
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
		uchCRCLo = auchCRCLo[uIndex];
	}
	return (uchCRCHi << 8 | uchCRCLo);
}

/// <summary>
///     MODBUS-RTU的读取线圈寄存器命令帧打包函数
/// </summary>
/// <param name="funcCode" type=unsigned char">
///     <para>
///         01或02或03或04功能号
///     </para>
/// </param>
/// <param name="slaveAdd" type=unsigned short">
///     <para>
///         MODBUS从站地址
///     </para>
/// </param>
/// <param name="startReg" type="unsigned short">
///     <para>
///         开始寄存器地址
///     </para>
/// </param>
/// <param name="regNums" type="unsigned short">
///     <para>
///         寄存器个数
///     </para>
/// </param>
/// <param name="buffer" type="BYTE类型的数组指针">
///     <para>
///         命令发送帧的缓存(不能小于8个字节长度）
///     </para>
/// </param>
/// <param name="recvBytes" type="unsigned short">
///     <para>
///         命令发送帧发送后，从站应该返回的数据帧的正常字节数
///     </para>
/// </param>
/// <returns>
///     命令帧的长度
/// </returns>
int Fun01020304Pacet(BYTE funcCode, unsigned short slaveAdd,unsigned short startReg, unsigned short regNums,BYTE* buffer, unsigned short* recvBytes)
{
	if(funcCode != 0x01 && funcCode != 0x02 && funcCode != 0x03 && funcCode != 0x04)
		return M_PacketErr;
	buffer[0] = slaveAdd;
	buffer[1] = funcCode;
	buffer[2] = HIBYTE(startReg);
	buffer[3] = LOBYTE(startReg);
	buffer[4] = HIBYTE(regNums);
	buffer[5] = LOBYTE(regNums);
	unsigned short crcValue = CRC16(buffer, 6);
	buffer[6] = HIBYTE(crcValue);
	buffer[7] = LOBYTE(crcValue);
	switch(funcCode)
	{
	case 0x01:
		if(regNums % 8 == 0)
			*recvBytes = 5 + regNums / 8;
		else
			*recvBytes = 5 + regNums / 8 + 1;
		break;
	case 0x02:
		if(regNums % 8 == 0)
			*recvBytes = 5 + regNums / 8;
		else
			*recvBytes = 5 + regNums / 8 + 1;
		break;
	case 0x03:
		*recvBytes = 5 + regNums * 2;
		break;
	case 0x04:
		*recvBytes = 5 + regNums * 2;
	}
	return 8;
}

/// <summary>
///     MODBUS-RTU的读取线圈寄存器返回帧解析包函数
/// </summary>
/// <param name="buffer" type=unsigned char">
///     <para>
///         要解包的返回帧缓存
///     </para>
/// </param>
/// <param name="bufferLen" type="unsigned short">
///     <para>
///         返回帧长度
///     </para>
/// </param>
/// <param name="regNums" type="unsigned short">
///     <para>
///         解包的接收缓存中线圈寄存器个数
///     </para>
/// </param>
/// <param name="slaveAdd" type="unsigned short">
///     <para>
///         接收帧中的从站地址
///     </para>
/// </param>
/// <param name="regValues" type="unsigned short*">
///     <para>
///         寄存器值的数组
///     </para>
/// </param>
/// <returns>
///     返回值：0-解析成功，!= 参考ModRetCode枚举类型
/// </returns>
int Fun01UnPacket(const BYTE *buffer, unsigned short bufferLen, unsigned short regNums, unsigned short *slaveAdd, unsigned short* regValues)
{
	unsigned short crcValue = CRC16(buffer, bufferLen-2);
	if(buffer[bufferLen - 2] != HIBYTE(crcValue) || buffer[bufferLen - 1] != LOBYTE(crcValue))
		return M_CRCError;
	*slaveAdd = buffer[0];
	if(buffer[1] != 0x01)
	{
		if(buffer[1] == 0x81)
		{
			return buffer[2];
		}
		else
			return M_UnPacketErr;
	}
    int datBytes = regNums / 8;
    if (regNums % 8 > 0)
        datBytes++;
    if (buffer[2] != datBytes)
        return M_UnPacketErr;
    int bitIndex = 0;
    for (int i = 0; i < datBytes; i++)
    {
        int data = (int)buffer[3 + i];
        for (int j = 0; j < 8; j++)
        {
            double valueDouble = pow(2, j);
            int valueByte = data & (int)valueDouble;
            if (valueByte > 0)
                regValues[bitIndex] = 1;
            else
                regValues[bitIndex] = 0;
            bitIndex++;
            if (bitIndex == regNums)
            {
                return M_SUCCESS;
            }
        }
    }
    return M_SUCCESS;
}

/// <summary>
///     MODBUS-RTU的读取数字量输入寄存器返回帧解析包函数
/// </summary>
/// <param name="buffer" type=unsigned char">
///     <para>
///         要解包的返回帧缓存
///     </para>
/// </param>
/// <param name="bufferLen" type="unsigned short">
///     <para>
///         返回帧长度
///     </para>
/// </param>
/// <param name="regNums" type="unsigned short">
///     <para>
///         解包的接收缓存中线圈寄存器个数
///     </para>
/// </param>
/// <param name="slaveAdd" type="unsigned short">
///     <para>
///         接收帧中的从站地址
///     </para>
/// </param>
/// <param name="regValues" type="unsigned short*">
///     <para>
///         寄存器值的数组
///     </para>
/// </param>
/// <returns>
///     返回值：0-解析成功，-1 CRC校验错误，>0 从站返回的错误码
/// </returns>
int Fun02UnPacket(const BYTE *buffer, unsigned short bufferLen, unsigned short regNums, unsigned short *slaveAdd, unsigned short* regValues)
{
	unsigned short crcValue = CRC16(buffer, bufferLen-2);
	if(buffer[bufferLen - 2] != HIBYTE(crcValue) || buffer[bufferLen - 1] != LOBYTE(crcValue))
		return M_CRCError;
	*slaveAdd = buffer[0];
	if(buffer[1] != 0x02)
	{
		if(buffer[1] == 0x82)
		{
			return buffer[2];
		}
		else
			return M_UnPacketErr;
	}

    int datBytes = regNums / 8;
    if (regNums % 8 > 0)
        datBytes++;
    if (buffer[2] != datBytes)
        return M_UnPacketErr;
    int bitIndex = 0;
    for (int i = 0; i < datBytes; i++)
    {
        int data = (int)buffer[3 + i];
        for (int j = 0; j < 8; j++)
        {
            double valueDouble = pow(2, j);
            int valueByte = data & (int)valueDouble;
            if (valueByte > 0)
                regValues[bitIndex] = 1;
            else
                regValues[bitIndex] = 0;
            bitIndex++;
            if (bitIndex == regNums)
            {
                return M_SUCCESS;
            }
        }
    }
    return M_SUCCESS;
}

/// <summary>
///     MODBUS-RTU的读取保持寄存器返回帧解析包函数
/// </summary>
/// <param name="buffer" type=unsigned char">
///     <para>
///         要解包的返回帧缓存
///     </para>
/// </param>
/// <param name="bufferLen" type="unsigned short">
///     <para>
///         返回帧长度
///     </para>
/// </param>
/// <param name="regNums" type="unsigned short">
///     <para>
///         解包的接收缓存中线圈寄存器个数
///     </para>
/// </param>
/// <param name="slaveAdd" type="unsigned short">
///     <para>
///         接收帧中的从站地址
///     </para>
/// </param>
/// <param name="regValues" type="unsigned short*">
///     <para>
///         寄存器值的数组
///     </para>
/// </param>
/// <param name="regFormat" type="RegFormat">
///     <para>
///         寄存器值的格式，参考头文件
///     </para>
/// </param>
/// <returns>
///     返回值：0-解析成功，-1 CRC校验错误，>0 从站返回的错误码
/// </returns>
int Fun03UnPacket(const BYTE* buffer, unsigned short bufferLen, unsigned short regNums, unsigned short* slaveAdd, double* regValues, RegFormat regFormat)
{
	unsigned short crcValue = CRC16(buffer, bufferLen-2);
	if(buffer[bufferLen - 2] != HIBYTE(crcValue) || buffer[bufferLen - 1] != LOBYTE(crcValue))
		return M_CRCError;
	*slaveAdd = buffer[0];
	if(buffer[1] != 0x03)
	{
		if(buffer[1] == 0x83)
		{
			return buffer[2];
		}
		else
			return M_UnPacketErr;
	}
	//寄存器个数和返回帧中的个数不同
	if(buffer[2] != regNums*2)
	{
		return M_UnPacketErr;
	}

	if(regFormat == UnSignedShort)
	{
		if(regNums != buffer[2] / 2)
			return -1;
		unsigned short* usPoint = nullptr;
		BYTE unShortBytes[2];
		unsigned short usTemp = 0;
		for(int i = 0; i < regNums; i++)
		{
			unShortBytes[0] = buffer[3+i*2+1];
			unShortBytes[1] = buffer[3+i*2];
			usPoint = (unsigned short*)unShortBytes;
			usTemp = *usPoint;
			regValues[i] = (double)(usTemp);
		}
		return M_SUCCESS;
	}
	if(regFormat == SignedShort)
	{
		if(regNums != buffer[2] / 2)
			return -1;
		short* usPoint = nullptr;
		BYTE shortBytes[2];
		short stTemp = 0;
		for(int i = 0; i < regNums; i++)
		{
			shortBytes[0] = buffer[3+i*2+1];
			shortBytes[1] = buffer[3+i*2];
			usPoint = (short*)shortBytes;
			stTemp = *usPoint;
			regValues[i] = (double)(stTemp);
		}
		return M_SUCCESS;
	}
	if(regFormat == Float4321)
	{
		float* ftPoint = nullptr;
		BYTE floatBytes[4];
		float ftTemp = 0;

		//接收的寄存器值没有4个字节则返回错误
		if(buffer[2] < 4)
			return M_UnPacketErr;

		//接收的寄存器值的字节个数不是4的整数倍则返回错误
		if(buffer[2] % 4 != 0)
			return M_UnPacketErr;

		//寄存器个数和接收数据不符合则返回错误
		if(regNums != buffer[2] / 2)
			return M_UnPacketErr;

		for(int i=0; i < regNums / 2; i++)
		{
			floatBytes[3] = buffer[3+i*4];
			floatBytes[2] = buffer[3+i*4+1];
			floatBytes[1] = buffer[3+i*4+2];
			floatBytes[0] = buffer[3+i*4+3];
			ftPoint = (float*)floatBytes;
			ftTemp = *ftPoint;
			regValues[i] = (double)ftTemp;
			ftPoint = nullptr;
		}
		return M_SUCCESS;
	}
	if(regFormat == Float2143)
	{
		float* ftPoint = nullptr;
		BYTE floatBytes[4];
		float ftTemp = 0;

		//接收的寄存器值没有4个字节则返回错误
		if(buffer[2] < 4)
			return M_UnPacketErr;

		//接收的寄存器值的字节个数不是4的整数倍则返回错误
		if(buffer[2] % 4 != 0)
			return M_UnPacketErr;

		//寄存器个数和接收数据不符合则返回错误
		if(regNums != buffer[2] / 2)
			return M_UnPacketErr;

		for(int i=0; i < regNums / 2; i++)
		{
			floatBytes[1] = buffer[3+i*4];
			floatBytes[0] = buffer[3+i*4+1];
			floatBytes[3] = buffer[3+i*4+2];
			floatBytes[2] = buffer[3+i*4+3];
			ftPoint = (float*)floatBytes;
			ftTemp = *ftPoint;
			regValues[i] = (double)ftTemp;
		}
		return M_SUCCESS;
	}
	if(regFormat == Float1234)
	{
		float* ftPoint = nullptr;
		BYTE floatBytes[4];
		float ftTemp = 0;

		//接收的寄存器值没有4个字节则返回错误
		if(buffer[2] < 4)
			return M_UnPacketErr;

		//接收的寄存器值的字节个数不是4的整数倍则返回错误
		if(buffer[2] % 4 != 0)
			return M_UnPacketErr;

		//寄存器个数和接收数据不符合则返回错误
		if(regNums != buffer[2] / 2)
			return M_UnPacketErr;

		for(int i=0; i < regNums / 2; i++)
		{
			floatBytes[0] = buffer[3+i*4];
			floatBytes[1] = buffer[3+i*4+1];
			floatBytes[2] = buffer[3+i*4+2];
			floatBytes[3] = buffer[3+i*4+3];
			ftPoint = (float*)floatBytes;
			ftTemp = *ftPoint;
			regValues[i] = (double)ftTemp;
		}
		return M_SUCCESS;
	}
	if(regFormat == Float3412)
	{
		float* ftPoint = nullptr;
		BYTE floatBytes[4];
		float ftTemp = 0;

		//接收的寄存器值没有4个字节则返回错误
		if(buffer[2] < 4)
			return M_UnPacketErr;

		//接收的寄存器值的字节个数不是4的整数倍则返回错误
		if(buffer[2] % 4 != 0)
			return M_UnPacketErr;

		//寄存器个数和接收数据不符合则返回错误
		if(regNums != buffer[2] / 2)
			return M_UnPacketErr;

		for(int i=0; i < regNums / 2; i++)
		{
			floatBytes[2] = buffer[3+i*4];
			floatBytes[3] = buffer[3+i*4+1];
			floatBytes[0] = buffer[3+i*4+2];
			floatBytes[1] = buffer[3+i*4+3];
			ftPoint = (float*)floatBytes;
			ftTemp = *ftPoint;
			regValues[i] = (double)ftTemp;
		}
		return M_SUCCESS;
	}
	if(regFormat == Double8_1)
	{
		double* dblPoint = nullptr;
		BYTE doubleBytes[4];
		float dblTemp = 0;

		//接收的寄存器值没有4个字节则返回错误
		if(buffer[2] < 8)
			return M_UnPacketErr;

		//接收的寄存器值的字节个数不是4的整数倍则返回错误
		if(buffer[2] % 8 != 0)
			return M_UnPacketErr;

		//寄存器个数和接收数据不符合则返回错误
		if(regNums != buffer[2] / 4)
			return M_UnPacketErr;

		for(int i=0; i < regNums / 2; i++)
		{
			doubleBytes[7] = buffer[3+i*4];
			doubleBytes[6] = buffer[3+i*4+1];
			doubleBytes[5] = buffer[3+i*4+2];
			doubleBytes[4] = buffer[3+i*4+3];
			doubleBytes[3] = buffer[3+i*4+4];
			doubleBytes[2] = buffer[3+i*4+5];
			doubleBytes[1] = buffer[3+i*4+6];
			doubleBytes[0] = buffer[3+i*4+7];
			dblPoint = (double*)doubleBytes;
			dblTemp = *dblPoint;
			regValues[i] = (double)dblTemp;
		}
		return M_SUCCESS;
	}
	if(regFormat == Double1_8)
	{
		double* dblPoint = nullptr;
		BYTE doubleBytes[4];
		float dblTemp = 0;

		//接收的寄存器值没有4个字节则返回错误
		if(buffer[2] < 8)
			return M_UnPacketErr;

		//接收的寄存器值的字节个数不是4的整数倍则返回错误
		if(buffer[2] % 8 != 0)
			return M_UnPacketErr;

		//寄存器个数和接收数据不符合则返回错误
		if(regNums != buffer[2] / 4)
			return M_UnPacketErr;

		for(int i=0; i < regNums / 2; i++)
		{
			doubleBytes[0] = buffer[3+i*4];
			doubleBytes[1] = buffer[3+i*4+1];
			doubleBytes[2] = buffer[3+i*4+2];
			doubleBytes[3] = buffer[3+i*4+3];
			doubleBytes[4] = buffer[3+i*4+4];
			doubleBytes[5] = buffer[3+i*4+5];
			doubleBytes[6] = buffer[3+i*4+6];
			doubleBytes[7] = buffer[3+i*4+7];
			dblPoint = (double*)doubleBytes;
			dblTemp = *dblPoint;
			regValues[i] = (double)dblTemp;
		}
		return M_SUCCESS;
	}

    return -1;
}

/// <summary>
///     MODBUS-RTU的读取输入寄存器返回帧解析包函数
/// </summary>
/// <param name="buffer" type=unsigned char">
///     <para>
///         要解包的返回帧缓存
///     </para>
/// </param>
/// <param name="bufferLen" type="unsigned short">
///     <para>
///         返回帧长度
///     </para>
/// </param>
/// <param name="regNums" type="unsigned short">
///     <para>
///         解包的接收缓存中线圈寄存器个数
///     </para>
/// </param>
/// <param name="slaveAdd" type="unsigned short">
///     <para>
///         接收帧中的从站地址
///     </para>
/// </param>
/// <param name="regValues" type="unsigned short*">
///     <para>
///         寄存器值的数组
///     </para>
/// </param>
/// <param name="regFormat" type="RegFormat">
///     <para>
///         寄存器值的格式，参考头文件
///     </para>
/// </param>
/// <returns>
///     返回值：0-解析成功，-1 CRC校验错误，>0 从站返回的错误码
/// </returns>
int Fun04UnPacket(const BYTE *buffer, unsigned short bufferLen, unsigned short regNums, unsigned short *slaveAdd, double* regValues, RegFormat regFormat)
{
	unsigned short crcValue = CRC16(buffer, bufferLen-2);
	if(buffer[bufferLen - 2] != HIBYTE(crcValue) || buffer[bufferLen - 1] != LOBYTE(crcValue))
		return M_CRCError;
	*slaveAdd = buffer[0];
	if(buffer[1] != 0x04)
	{
		if(buffer[1] == 0x84)
		{
			return buffer[2];
		}
		else
			return M_UnPacketErr;
	}

	//寄存器个数和返回帧中的个数不同
	if(buffer[2] != regNums*2)
	{
		return M_UnPacketErr;
	}

	if(regFormat == UnSignedShort)
	{
		if(regNums != buffer[2] / 2)
			return M_UnPacketErr;
		unsigned short* usPoint = nullptr;
		BYTE unShortBytes[2];
		unsigned short usTemp = 0;
		for(int i = 0; i < regNums; i++)
		{
			unShortBytes[0] = buffer[3+i*2+1];
			unShortBytes[1] = buffer[3+i*2];
			usPoint = (unsigned short*)unShortBytes;
			usTemp = *usPoint;
			regValues[i] = (double)(usTemp);
		}
		return M_SUCCESS;
	}
	if(regFormat == SignedShort)
	{
		if(regNums != buffer[2] / 2)
			return M_UnPacketErr;
		short* usPoint = nullptr;
		BYTE shortBytes[2];
		short stTemp = 0;
		for(int i = 0; i < regNums; i++)
		{
			shortBytes[0] = buffer[3+i*2+1];
			shortBytes[1] = buffer[3+i*2];
			usPoint = (short*)shortBytes;
			stTemp = *usPoint;
			regValues[i] = (double)(stTemp);
		}
		return M_SUCCESS;
	}
	if(regFormat == Float4321)
	{
		float* ftPoint = nullptr;
		BYTE floatBytes[4];
		float ftTemp = 0;

		//接收的寄存器值没有4个字节则返回错误
		if(buffer[2] < 4)
			return M_UnPacketErr;

		//接收的寄存器值的字节个数不是4的整数倍则返回错误
		if(buffer[2] % 4 != 0)
			return M_UnPacketErr;

		//寄存器个数和接收数据不符合则返回错误
		if(regNums != buffer[2] / 2)
			return M_UnPacketErr;

		for(int i=0; i < regNums / 2; i++)
		{
			floatBytes[3] = buffer[3+i*4];
			floatBytes[2] = buffer[3+i*4+1];
			floatBytes[1] = buffer[3+i*4+2];
			floatBytes[0] = buffer[3+i*4+3];
			ftPoint = (float*)floatBytes;
			ftTemp = *ftPoint;
			regValues[i] = (double)ftTemp;
		}
		return M_SUCCESS;
	}
	if(regFormat == Float2143)
	{
		float* ftPoint = nullptr;
		BYTE floatBytes[4];
		float ftTemp = 0;

		//接收的寄存器值没有4个字节则返回错误
		if(buffer[2] < 4)
			return M_UnPacketErr;

		//接收的寄存器值的字节个数不是4的整数倍则返回错误
		if(buffer[2] % 4 != 0)
			return M_UnPacketErr;

		//寄存器个数和接收数据不符合则返回错误
		if(regNums != buffer[2] / 2)
			return M_UnPacketErr;

		for(int i=0; i < regNums / 2; i++)
		{
			floatBytes[1] = buffer[3+i*4];
			floatBytes[0] = buffer[3+i*4+1];
			floatBytes[3] = buffer[3+i*4+2];
			floatBytes[2] = buffer[3+i*4+3];
			ftPoint = (float*)floatBytes;
			ftTemp = *ftPoint;
			regValues[i] = (double)ftTemp;
		}
		return M_SUCCESS;
	}
	if(regFormat == Float1234)
	{
		float* ftPoint = nullptr;
		BYTE floatBytes[4];
		float ftTemp = 0;

		//接收的寄存器值没有4个字节则返回错误
		if(buffer[2] < 4)
			return M_UnPacketErr;

		//接收的寄存器值的字节个数不是4的整数倍则返回错误
		if(buffer[2] % 4 != 0)
			return M_UnPacketErr;

		//寄存器个数和接收数据不符合则返回错误
		if(regNums != buffer[2] / 2)
			return M_UnPacketErr;

		for(int i=0; i < regNums / 2; i++)
		{
			floatBytes[0] = buffer[3+i*4];
			floatBytes[1] = buffer[3+i*4+1];
			floatBytes[2] = buffer[3+i*4+2];
			floatBytes[3] = buffer[3+i*4+3];
			ftPoint = (float*)floatBytes;
			ftTemp = *ftPoint;
			regValues[i] = (double)ftTemp;
		}
		return M_SUCCESS;
	}
	if(regFormat == Float3412)
	{
		float* ftPoint = nullptr;
		BYTE floatBytes[4];
		float ftTemp = 0;

		//接收的寄存器值没有4个字节则返回错误
		if(buffer[2] < 4)
			return M_UnPacketErr;

		//接收的寄存器值的字节个数不是4的整数倍则返回错误
		if(buffer[2] % 4 != 0)
			return M_UnPacketErr;

		//寄存器个数和接收数据不符合则返回错误
		if(regNums != buffer[2] / 2)
			return M_UnPacketErr;

		for(int i=0; i < regNums / 2; i++)
		{
			floatBytes[2] = buffer[3+i*4];
			floatBytes[3] = buffer[3+i*4+1];
			floatBytes[0] = buffer[3+i*4+2];
			floatBytes[1] = buffer[3+i*4+3];
			ftPoint = (float*)floatBytes;
			ftTemp = *ftPoint;
			regValues[i] = (double)ftTemp;
		}
		return M_SUCCESS;
	}
	if(regFormat == Double8_1)
	{
		double* dblPoint = nullptr;
		BYTE doubleBytes[8];
		float dblTemp = 0;

		//接收的寄存器值没有4个字节则返回错误
		if(buffer[2] < 8)
			return M_UnPacketErr;

		//接收的寄存器值的字节个数不是4的整数倍则返回错误
		if(buffer[2] % 8 != 0)
			return M_UnPacketErr;

		//寄存器个数和接收数据不符合则返回错误
		if(regNums != buffer[2] / 4)
			return M_UnPacketErr;

		for(int i=0; i < regNums / 2; i++)
		{
			doubleBytes[7] = buffer[3+i*4];
			doubleBytes[6] = buffer[3+i*4+1];
			doubleBytes[5] = buffer[3+i*4+2];
			doubleBytes[4] = buffer[3+i*4+3];
			doubleBytes[3] = buffer[3+i*4+4];
			doubleBytes[2] = buffer[3+i*4+5];
			doubleBytes[1] = buffer[3+i*4+6];
			doubleBytes[0] = buffer[3+i*4+7];
			dblPoint = (double*)doubleBytes;
			dblTemp = *dblPoint;
			regValues[i] = (double)dblTemp;
		}
		return M_SUCCESS;
	}
	if(regFormat == Double1_8)
	{
		double* dblPoint = nullptr;
		BYTE doubleBytes[8];
		float dblTemp = 0;

		//接收的寄存器值没有4个字节则返回错误
		if(buffer[2] < 8)
			return M_UnPacketErr;

		//接收的寄存器值的字节个数不是4的整数倍则返回错误
		if(buffer[2] % 8 != 0)
			return M_UnPacketErr;

		//寄存器个数和接收数据不符合则返回错误
		if(regNums != buffer[2] / 4)
			return M_UnPacketErr;

		for(int i=0; i < regNums / 2; i++)
		{
			doubleBytes[0] = buffer[3+i*4];
			doubleBytes[1] = buffer[3+i*4+1];
			doubleBytes[2] = buffer[3+i*4+2];
			doubleBytes[3] = buffer[3+i*4+3];
			doubleBytes[4] = buffer[3+i*4+4];
			doubleBytes[5] = buffer[3+i*4+5];
			doubleBytes[6] = buffer[3+i*4+6];
			doubleBytes[7] = buffer[3+i*4+7];
			dblPoint = (double*)doubleBytes;
			dblTemp = *dblPoint;
			regValues[i] = (double)dblTemp;
		}
		return M_SUCCESS;
	}

    return M_UnPacketErr;
}

/// <summary>
///     MODBUS-RTU的写入单个线圈寄存器命令帧打包函数
/// </summary>
/// <param name="slaveAdd" type=unsigned short">
///     <para>
///         MODBUS从站地址
///     </para>
/// </param>
/// <param name="startReg" type="unsigned short">
///     <para>
///         开始寄存器地址
///     </para>
/// </param>
/// <param name="openOrclose" type="BYTE">
///     <para>
///         线圈闭合还是断开，0：断开，非0：闭合
///     </para>
/// </param>
/// <param name="buffer" type="BYTE类型的数组指针">
///     <para>
///         命令发送帧的缓存(不能小于8个字节长度）
///     </para>
/// </param>
/// <returns>
///     命令帧的长度
/// </returns>
int Fun05Pack(unsigned short slaveAdd, unsigned short startReg, BYTE openOrclose, BYTE* buffer)
{
	buffer[0] = slaveAdd;
	buffer[1] = 0x05;
	buffer[2] = HIBYTE(startReg);
	buffer[3] = LOBYTE(startReg);
	if(openOrclose !=0)
	{
		buffer[4] = 0xFF;
		buffer[5] = 0x00;
	}
	else
	{
		buffer[4] = 0;
		buffer[5] = 0;
	}
	unsigned short crcValue = CRC16(buffer, 6);
	buffer[6] = HIBYTE(crcValue);
	buffer[7] = LOBYTE(crcValue);
	return 8;
}

/// <summary>
///     MODBUS-RTU的写入单个线圈寄存器命令返回帧解包函数
/// </summary>
/// <param name="slaveAdd" type=unsigned short">
///     <para>
///         MODBUS从站地址
///     </para>
/// </param>
/// <param name="buffer" type="const BYTE*">
///     <para>
///         接收数据帧缓存数组
///     </para>
/// </param>
/// <param name="bufferLen" type="unsigned short">
///     <para>
///         接收缓存数组的长度
///     </para>
/// </param>
/// <param name="slaveAdd" type="unsigned short*">
///     <para>
///         返回的数据帧中的从站地址
///     </para>
/// </param>
/// <param name="regAdd" type="unsigned short*">
///     <para>
///         返回的数据帧中的线圈寄存器地址
///     </para>
/// </param>
/// <param name="regValue" type="BYTE*">
///     <para>
///         返回的数据帧中的寄存器值，0：断开，非0：闭合
///     </para>
/// </param>
/// <returns>
///     0：正常，-1：错误
/// </returns>
int Fun05UnPack(const BYTE* buffer, unsigned short bufferLen, unsigned short *slaveAdd, unsigned short *regAdd, BYTE* regValue)
{
	if(bufferLen < 5)
		return M_CRCError;
	unsigned short crcValue = CRC16(buffer, bufferLen-2);
	if(buffer[bufferLen - 2] != HIBYTE(crcValue) || buffer[bufferLen - 1] != LOBYTE(crcValue))
		return M_CRCError;
	*slaveAdd = buffer[0];
	if(buffer[1] != 0x05)
	{
		if(buffer[1] == 0x85)
		{
			return buffer[2];
		}
		else
		{
			return M_UnPacketErr;
		}
	}
	BYTE shortByteTemp[2];
	shortByteTemp[1] = buffer[2];
	shortByteTemp[0] = buffer[3];
	unsigned short* usPTemp = (unsigned short*)shortByteTemp;
	*regAdd = *usPTemp;
	if(buffer[4] == 0 && buffer[5] == 0)
	{
		*regValue = 0;
	}
	else
	{
		if(buffer[4] == 0xFF && buffer[5] == 0x0)
			*regValue = 1;
		else
			return M_UnPacketErr;
	}
	return M_SUCCESS;
}

/// <summary>
///     MODBUS-RTU的写入多个线圈寄存器命令帧打包函数
/// </summary>
/// <param name="slaveAdd" type=unsigned short">
///     <para>
///         MODBUS从站地址
///     </para>
/// </param>
/// <param name="startReg" type="unsigned short">
///     <para>
///         开始寄存器地址
///     </para>
/// </param>
/// <param name="regNums" type="unsigned short">
///     <para>
///         线圈寄存器个数
///     </para>
/// </param>
/// <param name="regValue" type="const BYTE*">
///     <para>
///         线圈寄存器的值数组
///     </para>
/// </param>
/// <param name="buffer" type="BYTE类型的数组指针">
///     <para>
///         命令发送帧的缓存(不能小于10个字节长度）
///     </para>
/// </param>
/// <returns>
///     命令帧的长度
/// </returns>
int Fun0FPack(unsigned short slaveAdd, unsigned short startReg, unsigned short regNums, const BYTE* regValues, BYTE* buffer)
{
	if(regNums > 0x7B0)
		return M_PacketErr;
	buffer[0] = slaveAdd;
	buffer[1] = 0x0F;
	buffer[2] = HIBYTE(startReg);
	buffer[3] = LOBYTE(startReg);
	buffer[4] = HIBYTE(regNums);
	buffer[5] = LOBYTE(regNums);
	if(regNums <=8)
	{
		buffer[6] = 1;
		BYTE btTemp = 0;
		for(int i = 0; i < regNums; i++)
		{
			if(regValues[i] != 0)
			{
				btTemp = btTemp | (BYTE)pow(2,i);
			}
		}
		buffer[7] = btTemp;
		unsigned short crcValue = CRC16(buffer,8);
		buffer[8] = HIBYTE(crcValue);
		buffer[9] = LOBYTE(crcValue);
		return 10;
	}
	else
	{
		BYTE byteNums = 0;
		if(regNums % 8 == 0)
			byteNums = regNums / 8;
		else
			byteNums = regNums / 8 + 1;
		buffer[6] = byteNums;
		int nowRegNo = 0;
		for(int i = 0; i < byteNums; i++)
		{
			buffer[7+i] = 0;
			for(int j = 0; j < 8; j++)
			{
				if(regValues[nowRegNo] != 0)
				{
					buffer[7+i] = buffer[7+i] | (BYTE)pow(2,j);
				}
				nowRegNo++;
				if(nowRegNo == regNums)
					break;
			}
			if(nowRegNo == regNums)
				break;
		}
		unsigned short crcValue = CRC16(buffer, 7+byteNums);
		buffer[7+byteNums] = HIBYTE(crcValue);
		buffer[8+byteNums] = LOBYTE(crcValue);
		return  9+byteNums;
	}
}

/// <summary>
///     MODBUS-RTU的写入多个线圈寄存器命令返回帧解包函数
/// </summary>
/// <param name="slaveAdd" type=unsigned short">
///     <para>
///         MODBUS从站地址
///     </para>
/// </param>
/// <param name="buffer" type="const BYTE*">
///     <para>
///         接收数据帧缓存数组
///     </para>
/// </param>
/// <param name="bufferLen" type="unsigned short">
///     <para>
///         接收缓存数组的长度
///     </para>
/// </param>
/// <param name="slaveAdd" type="unsigned short*">
///     <para>
///         返回的数据帧中的从站地址
///     </para>
/// </param>
/// <param name="startReg" type="unsigned short*">
///     <para>
///         返回的数据帧中的线圈寄存器开始地址
///     </para>
/// </param>
/// <param name="regNums" type="unsigned short*">
///     <para>
///         返回的数据帧中的寄存器个数
///     </para>
/// </param>
/// <returns>
///     0：正常，-1：错误
/// </returns>
int Fun0FUnPacket(const BYTE* buffer, int bufferLen, unsigned short* slaveAdd, unsigned short* startReg, unsigned short* regNums)
{
	if(bufferLen < 5)
		return M_CRCError;
	unsigned short crcValue = CRC16(buffer, bufferLen-2);
	if(HIBYTE(crcValue) != buffer[bufferLen-2] || LOBYTE(crcValue) != buffer[bufferLen-1])
		return M_CRCError;
	*slaveAdd = buffer[0];
	if(buffer[1] == 0x8F)
		return buffer[2];
	if(buffer[1] != 0x0F)
		return M_UnPacketErr;
	BYTE byteArray[2];
	byteArray[1] = buffer[2];
	byteArray[0] = buffer[3];
	unsigned short* usPoint = (unsigned short*)byteArray;
	*startReg = *usPoint;

	byteArray[1] = buffer[4];
	byteArray[0] = buffer[5];
	usPoint = (unsigned short*)byteArray;
	*regNums = *usPoint;

	return M_SUCCESS;
}

/// <summary>
///     MODBUS-RTU的写入多个保持寄存器命令帧打包函数
/// </summary>
/// <param name="slaveAdd" type=unsigned short">
///     <para>
///         MODBUS从站地址
///     </para>
/// </param>
/// <param name="startReg" type="unsigned short">
///     <para>
///         开始寄存器地址
///     </para>
/// </param>
/// <param name="regNums" type="unsigned short">
///     <para>
///         寄存器个数
///     </para>
/// </param>
/// <param name="regValue" type="const double*">
///     <para>
///         寄存器的值数组
///     </para>
/// </param>
/// <param name="buffer" type="BYTE类型的数组指针">
///     <para>
///         命令发送帧的缓存(不能小于10个字节长度）
///     </para>
/// </param>
/// <param name="regFormat" type="RegFormat">
///     <para>
///         寄存器值的数据类型和排列格式
///     </para>
/// </param>
/// <returns>
///     命令帧的长度
/// </returns>
int Fun10Packet(unsigned short slaveAdd, unsigned short startReg, unsigned short regNums, const double* regValues, BYTE* buffer, RegFormat regFormat)
{
	if(regNums > 0x78 || regNums < 1)
		return M_PacketErr;
	buffer[0] = slaveAdd;
	buffer[1] = 0x10;
	buffer[2] = HIBYTE(startReg);
	buffer[3] = LOBYTE(startReg);
	buffer[4] = HIBYTE(regNums);
	buffer[5] = LOBYTE(regNums);
	buffer[6] = 2*regNums;
	if(regFormat == UnSignedShort)
	{
		for(int i = 0; i < regNums; i++)
		{
			unsigned short usTemp = (unsigned short)regValues[i];
			buffer[7+i*2] = HIBYTE(usTemp);
			buffer[7+i*2+1] = LOBYTE(usTemp);
		}
		unsigned short crcValue = CRC16(buffer, 7+2*regNums);
		buffer[7+2*regNums] = HIBYTE(crcValue);
		buffer[7+2*regNums+1] = LOBYTE(crcValue);
		return 9+2*regNums;
	}

	if(regFormat == SignedShort)
	{
		for(int i = 0; i < regNums; i++)
		{
			short usTemp = (unsigned short)regValues[i];
			buffer[7+i*2] = HIBYTE(usTemp);
			buffer[7+i*2+1] = LOBYTE(usTemp);
		}
		unsigned short crcValue = CRC16(buffer, 7+2*regNums);
		buffer[7+2*regNums] = HIBYTE(crcValue);
		buffer[7+2*regNums+1] = LOBYTE(crcValue);
		return 9+2*regNums;
	}

	if(regFormat == Float4321)
	{
		if(regNums % 2 != 0)
			return M_PacketErr;
		int floatNums = regNums / 2;
		for(int i = 0; i < floatNums; i++)
		{
			float fTemp = (float)regValues[i];
			BYTE* fpPoint = (BYTE*)&fTemp;
			buffer[7+i*4] = fpPoint[3];
			buffer[7+i*4+1] = fpPoint[2];
			buffer[7+i*4+2] = fpPoint[1];
			buffer[7+i*4+3] = fpPoint[0];
		}
		unsigned short crcValue = CRC16(buffer, 7+2*regNums);
		buffer[7+2*regNums] = HIBYTE(crcValue);
		buffer[7+2*regNums+1] = LOBYTE(crcValue);
		return 9+2*regNums;
	}

	if(regFormat == Float2143)
	{
		if(regNums % 2 != 0)
			return M_PacketErr;
		int floatNums = regNums / 2;
		for(int i = 0; i < floatNums; i++)
		{
			float fTemp = (float)regValues[i];
			BYTE* fpPoint = (BYTE*)&fTemp;
			buffer[7+i*4] = fpPoint[1];
			buffer[7+i*4+1] = fpPoint[0];
			buffer[7+i*4+2] = fpPoint[3];
			buffer[7+i*4+3] = fpPoint[2];
		}
		unsigned short crcValue = CRC16(buffer, 7+2*regNums);
		buffer[7+2*regNums] = HIBYTE(crcValue);
		buffer[7+2*regNums+1] = LOBYTE(crcValue);
		return 9+2*regNums;
	}

	if(regFormat == Float1234)
	{
		if(regNums % 2 != 0)
			return M_PacketErr;
		int floatNums = regNums / 2;
		for(int i = 0; i < floatNums; i++)
		{
			float fTemp = (float)regValues[i];
			BYTE* fpPoint = (BYTE*)&fTemp;
			buffer[7+i*4] = fpPoint[0];
			buffer[7+i*4+1] = fpPoint[1];
			buffer[7+i*4+2] = fpPoint[2];
			buffer[7+i*4+3] = fpPoint[3];
		}
		unsigned short crcValue = CRC16(buffer, 7+2*regNums);
		buffer[7+2*regNums] = HIBYTE(crcValue);
		buffer[7+2*regNums+1] = LOBYTE(crcValue);
		return 9+2*regNums;
	}

	if(regFormat == Float3412)
	{
		if(regNums % 2 != 0)
			return M_PacketErr;
		int floatNums = regNums / 2;
		for(int i = 0; i < floatNums; i++)
		{
			float fTemp = (float)regValues[i];
			BYTE* fpPoint = (BYTE*)&fTemp;
			buffer[7+i*4] = fpPoint[2];
			buffer[7+i*4+1] = fpPoint[3];
			buffer[7+i*4+2] = fpPoint[0];
			buffer[7+i*4+3] = fpPoint[2];
		}
		unsigned short crcValue = CRC16(buffer, 7+2*regNums);
		buffer[7+2*regNums] = HIBYTE(crcValue);
		buffer[7+2*regNums+1] = LOBYTE(crcValue);
		return 9+2*regNums;
	}

	if(regFormat == Double8_1)
	{
		if(regNums % 4 != 0)
			return M_PacketErr;
		int doubleNums = regNums / 4;
		for(int i = 0; i < doubleNums; i++)
		{
			double fTemp = regValues[i];
			BYTE* dbPoint = (BYTE*)&fTemp;
			buffer[7+i*4] = dbPoint[7];
			buffer[7+i*4+1] = dbPoint[6];
			buffer[7+i*4+2] = dbPoint[5];
			buffer[7+i*4+3] = dbPoint[4];
			buffer[7+i*4+4] = dbPoint[3];
			buffer[7+i*4+5] = dbPoint[2];
			buffer[7+i*4+6] = dbPoint[1];
			buffer[7+i*4+7] = dbPoint[0];
		}
		unsigned short crcValue = CRC16(buffer, 7+2*regNums);
		buffer[7+2*regNums] = HIBYTE(crcValue);
		buffer[7+2*regNums+1] = LOBYTE(crcValue);
		return 9+2*regNums;
	}

	if(regFormat == Double1_8)
	{
		if(regNums % 4 != 0)
			return M_PacketErr;
		int doubleNums = regNums / 4;
		for(int i = 0; i < doubleNums; i++)
		{
			double fTemp = regValues[i];
			BYTE* dbPoint = (BYTE*)&fTemp;
			buffer[7+i*4] = dbPoint[0];
			buffer[7+i*4+1] = dbPoint[1];
			buffer[7+i*4+2] = dbPoint[2];
			buffer[7+i*4+3] = dbPoint[3];
			buffer[7+i*4+4] = dbPoint[4];
			buffer[7+i*4+5] = dbPoint[5];
			buffer[7+i*4+6] = dbPoint[6];
			buffer[7+i*4+7] = dbPoint[7];
		}
		unsigned short crcValue = CRC16(buffer, 7+2*regNums);
		buffer[7+2*regNums] = HIBYTE(crcValue);
		buffer[7+2*regNums+1] = LOBYTE(crcValue);
		return 9+2*regNums;
	}
	return M_PacketErr;
}

/// <summary>
///     MODBUS-RTU的写入多个保持寄存器命令返回帧解包函数
/// </summary>
/// <param name="slaveAdd" type=unsigned short">
///     <para>
///         MODBUS从站地址
///     </para>
/// </param>
/// <param name="buffer" type="const BYTE*">
///     <para>
///         接收数据帧缓存数组
///     </para>
/// </param>
/// <param name="bufferLen" type="unsigned short">
///     <para>
///         接收缓存数组的长度
///     </para>
/// </param>
/// <param name="slaveAdd" type="unsigned short*">
///     <para>
///         返回的数据帧中的从站地址
///     </para>
/// </param>
/// <param name="startReg" type="unsigned short*">
///     <para>
///         返回的数据帧中的保持寄存器开始地址
///     </para>
/// </param>
/// <param name="regNums" type="unsigned short*">
///     <para>
///         返回的数据帧中的寄存器个数
///     </para>
/// </param>
/// <returns>
///     0：正常，-1：错误
/// </returns>
int Fun10UnPacket(const BYTE* buffer, int bufferLen, unsigned short* slaveAdd, unsigned short* startReg, unsigned short* regNums)
{
	if(bufferLen < 5)
		return M_CRCError;
	unsigned short crcValue = CRC16(buffer, bufferLen-2);
	if(HIBYTE(crcValue) != buffer[bufferLen-2] || LOBYTE(crcValue) != buffer[bufferLen-1])
		return M_CRCError;
	*slaveAdd = buffer[0];
	if(buffer[1] == 0x90)
		return buffer[2];
	if(buffer[1] != 0x10)
		return M_UnPacketErr;
	BYTE byteArray[2];
	byteArray[1] = buffer[2];
	byteArray[0] = buffer[3];
	unsigned short* usPoint = (unsigned short*)byteArray;
	*startReg = *usPoint;

	byteArray[1] = buffer[4];
	byteArray[0] = buffer[5];
	usPoint = (unsigned short*)byteArray;
	*regNums = *usPoint;

	return M_SUCCESS;
}



