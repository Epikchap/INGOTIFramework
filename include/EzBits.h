#include <stdint.h>

//Pretty simple; I guess you could say its...EZ

//Bit functions are made for unsigned 1 byte integer types.



//Works on reading bits 0-7 in a byte. 0 = bit #1, 7 = bit #8.
inline bool ReadBit(uint8_t TargetByte, unsigned char BitPlace)
{
		return 1 & (TargetByte >> BitPlace);	
}



inline void SetBit(uint8_t* ByteToWrite, uint8_t BitPlace, bool Value)
{
	//Ty GeeksForGeeks
	if(Value == true)
	{
		*ByteToWrite = (*ByteToWrite | (1 << BitPlace));
	}
	else
	{
		*ByteToWrite = (*ByteToWrite & (~(1 << BitPlace)));
	}

}
