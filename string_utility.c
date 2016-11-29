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
unsigned int reverse32(unsigned int data)
{
        unsigned char byte[4];
        unsigned int temp;

        byte[0] = (unsigned char) (data>>24);
        byte[1] = (unsigned char) (data>>16);
        byte[2] = (unsigned char) (data>>8);
        byte[3] = (unsigned char)  data;

        memcpy( &temp, byte, 4);

        return temp;

}
unsigned short reverse16(unsigned short data)
{
        return (unsigned char)(data>>8) + (data<<8);
}
/**********************************************************************/
/** 最简单的异或和校验
 * 
 * @param pStrEntry 待校验数据入口
 * @param kStrLen   待校验数据长度
 * @return 校验结果
 * */
char calcXOR( char * pStrEntry, int kStrLen )
{
        int k=0, kCs = 0;
        for ( ; k<kStrLen; k++ )
                kCs ^= pStrEntry[k];

        return kCs;
}
        

/***********************************************************************
 * IP INFO
 **********************************************************************/

int getIpInfo(struct sockaddr_in ClientAddr, int LocalFd, char ** LocalIp, char **ClientIp)
{
        /** @note 取得本地IP*/
        struct sockaddr_in MyAddr;
        memset(&MyAddr,0,sizeof(MyAddr));
        socklen_t lenIP = sizeof(MyAddr);

        getsockname(LocalFd,(struct sockaddr*)&MyAddr, &lenIP);
         /*把网络地址转换为.分形式的字符串形式*/
        char * kIp  = inet_ntoa(MyAddr.sin_addr);
        int AddrLen = strlen(kIp);
        *LocalIp = (char *)malloc(AddrLen+1);
        memcpy(*LocalIp, kIp, AddrLen);
        (*LocalIp)[AddrLen] = '\0';

        /** @note 取得客户端IP */
        *ClientIp = inet_ntoa(ClientAddr.sin_addr);
        return ntohs(ClientAddr.sin_port);
}

/***********************************************************************
 * 字符串操作
 **********************************************************************/

/** 类似strstr函数，从字符串haystack中寻找needle第一次出现的位置
 * 
 * @param haystack 需要查找的字符原串
 * @param wLen     查找字符原串总长
 * @param needle   需匹配的字符原串
 * @param wLen     匹配字符原串总长
 * @return    字符串haystack中寻找needle第一次出现的位置
 * @attention 无视结束符，适合在字节数组中查找
 */
unsigned char * strstr_Arr(unsigned char* haystack, int wLen, unsigned char* needle, int qLen)
{
        if (   ( NULL == haystack )
                || ( NULL == needle   )
                || ( 0 >= wLen )
                || ( 0 >= qLen )   )
                return NULL;

        int i=0, j=0;

        /** @note 剩余字节不足，不予判断 */
        for(i=0; (i<wLen) && ((wLen-i) >= qLen); ++i, ++haystack)
        {
                unsigned char *h = haystack;
                unsigned char *n = needle;

                for(j=0; j<qLen; ++j,++n,++h)
                        if(*h != *n) break;

                if(j == qLen) return haystack;
        }
        return NULL;
}






/** 取出前后空格
 * 
 * @param string 待处理字符串
 * @return   处理后的字符串
 */
char * string_trim(char * string)
{
        char * copy = string;

        char *end;
        // Trim leading space
        while(isspace(*copy)) copy++;

        // Trim trailing space
        end = copy + strlen(copy) - 1;
        while(end > copy && isspace(*end)) end--;

        // Write new null terminator
        *(end+1) = 0;

        return copy;
}

/** 判断字符串中是否包含空格
 * 
 * @param string 待处理字符串
 * @return  处理结果
 */
int string_emSpace(char * string)
{
        char * copy = string;
        char * end  = copy + strlen(copy) - 1;

        while(end > copy)
        {
                if(isspace(*copy))
                        return -1;
                copy++;
        }

        return 0;
}

/** 指定长度字符串转换为无符号整形
 * 
 * @param string 待处理字符串
 * @return  处理结果
 */
unsigned int string2uint(unsigned char * string, int len, T_Arch Endianness)
{
        int i;
        unsigned int result = 0;
        if(Endianness == BENDIAN)
                for(i=0; i<len; i++)
                {
                        result += string[i]*(1<<(8*(len-i-1)));
                }
        else
                for(i=0; i<len; i++)
                        result += string[i]*(1<<(8*i));

        return result;
}

/***********************************************************************
 * 系统显示操作
 **********************************************************************/

/** 以hex方式显示数组到指定终端
 * 
 * @param ptty 终端位置
 * @param pData 数据
 * @param len 数据长度
 * @return  无
 */
void DispHex2tty(char * ptty, unsigned char * pData, int len)
{
        FILE *Ptr = fopen(ptty, "w");

        int i;
        for(i=0; i<len; i++)
        {
                fprintf(Ptr, "%.2X-", pData[i]);
                if(i%16==15)
                        fprintf(Ptr, "\n");
                else if(i%16==7)
                        fprintf(Ptr, "  ");
        }
        fprintf(Ptr, "\n  L@0x%X\n", len);
        fprintf(Ptr, "---------------------\n");
        fclose(Ptr);
}




