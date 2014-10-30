class Cat9555
{
public:
	int GetCAT9555(int channel,unsigned char port,unsigned char *level);
	int SetCAT9555(int channel,unsigned char port,unsigned char level);
private:
	HANDLE h;
};
