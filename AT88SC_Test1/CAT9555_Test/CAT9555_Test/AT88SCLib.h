class AT88SC
{
public:
	int AT88SC::Write(int addr,unsigned char *data,int len);
	int AT88SC::Read(int addr,unsigned char *data,int len);
	int AT88SC::ReadID(unsigned char *id);
	int AT88SC::ConfigKey(unsigned char *G,unsigned char *pw);
private:
	HANDLE h;
};
