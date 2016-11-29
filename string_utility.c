/***********************************************************************
 * 字节序操作
 **********************************************************************/
T_Arch checkNativeEndianness()
{
        unsigned short a=0x1234;
        if (*((unsigned char *)&a)==0x12)
                return BENDIAN;
        else
                return LENDIAN;
}

