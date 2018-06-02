#include <string.h>	
#include <stdio.h>	
#include "hash.h"

unsigned long ELfHash(const unsigned char* name)
{
	unsigned long h = 0;
	unsigned long g = 0;

	while (*name)
	{
		h = (h << 4) + *name++;
		if ((g = h & 0xF0000000))
		{
			h ^= g >> 24;
		}

		h &= ~g;
	}

	return h;
}

#include <limits.h>
#define BITS_IN_int (sizeof(int)*CHAR_BIT)
#define THREE_QUARTERS ((int)((BITS_IN_int * 3) / 4))
#define ONE_EIGHTH ((int)(BITS_IN_int/8))
#define HIGH_BITS (~((unsigned int)(~0) >> ONE_EIGHTH))

unsigned int HashPJW(const char* datum)
{
	unsigned int hash_value;
	unsigned int i;	
	
	for (hash_value = 0; *datum; ++datum)
	{
		hash_value = (hash_value << ONE_EIGHTH) + *datum;
	
		if ((i = hash_value & HIGH_BITS) != 0)
		{
			hash_value = (hash_value ^ (i >> THREE_QUARTERS)) & ~HIGH_BITS;
		}
	}

	return hash_value;
}

	
unsigned int HashPJW1(const char* datum)
{
	unsigned char ucVcardno[8];  
	unsigned int hash_value=0;							
	unsigned int i;	
	unsigned char a;	
	
	for(i=0;i<strlen(datum);i++)
	{			
		a = *(datum+i) - '0';
		printf("%d ",a);			
		ucVcardno[i] = a;	
	}
	
	printf("\n");			

	for(i=0;i<strlen(datum);i++)			
	{	
		hash_value += ucVcardno[i];							
	}			
		
	return hash_value;
}



unsigned long HashElf ( const char *name )				
{  	
   unsigned long   h = 0, g;  
   while ( *name )	
	{  
	   //h����4λ���������8λ����nameָ����ַ�  
	   h = ( h << 4 ) + *name++;  
	   //��������λ��Ϊ�㣬��h����ַ��ﵽ7������8���ַ����޷���š�g���h���4λ��29-32λ��	
	   g = h & 0xF0000000;	
	   if ( g != 0 )		  		
	   {  	
		   //h��5-8λ��g�������λ��λ���  
		   h ^= g >> 24;		
	   }  
	   //���h�������λ  
	   h &= ~g;  
	}  
   //����h	
   return h;  
}  




