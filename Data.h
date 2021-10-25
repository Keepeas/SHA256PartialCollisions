#ifndef DATA_H
#define DATA_H

#include <time.h>
#include <iostream>
#include <openssl/sha.h>

class Data
{
public:
	unsigned char in[32];
	unsigned char out[32];

	bool operator<(const Data& a) const
	{
		for(int i = 31; i>=0; i--)
		{
			if(out[i] < a.out[i])
				return true;
			else if(out[i] > a.out[i])
				return false;
		}
		return false;
	}

	Data()
	{

	}

	Data(Data* a)
	{
		for(int i = 0; i< 32 ; i++)
		{
			in[i] = a->in[i];
			out[i] = a->out[i];
		}
	}

	void Inc()
	{
		in[31]++;
		if(in[31] ==0)
		{
			in[30]++;
			if(in[30]==0)
			{
				in[29]++;
			}
		}
	}
	void Rand()
	{
		for(int i = 0; i<32; i++)
		{
			in[i] = (unsigned int)(rand()%256);
		}
	}
	void Hash()
	{
		SHA256_CTX sha256;
		SHA256_Init(&sha256);
		SHA256_Update(&sha256, in, 32);
		SHA256_Final(out, &sha256);
	}
};

#endif
