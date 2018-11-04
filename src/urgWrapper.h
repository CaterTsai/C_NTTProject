#pragma once

#include "ofMain.h"
#include "Urg_driver.h"

using namespace qrk;

class urgWrapper : ofThread
{
public:
	bool conn(string ip, int port);	
	void draw(float ratio);

	void start(int startDeg = -135, int endDeg = 135, int skip = 0);
	void stop();

	vector<long>& getData();
	int getDataSize();
	bool hasNewData();

	void enableSmooth(int size);
	void disableSmooth();

private:
	void threadedFunction();
	void smoothing();

private:
	bool _isConn, _isStart, _hasNewData;
	
	int _startDeg, _endDeg;
	int _dataSize;
	long _timeStamp;
	vector<long> _data;
	Urg_driver _urg;

	bool _needSmooth;
	int _smoothSize;


#pragma region Singleton
//-------------------
//Singleton
private:
	urgWrapper()
		:_isConn(false)
		,_isStart(false)
		,_hasNewData(false)
		,_needSmooth(false)
	{
	};
	~urgWrapper()
	{
		urgWrapper::destroy();
	}
	urgWrapper(urgWrapper const&);
	void operator=(urgWrapper const&);

public:
	static urgWrapper* getInstance();
	static void destroy();

private:
	static urgWrapper *pInstance;
#pragma endregion

	
};