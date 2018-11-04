#include "urgWrapper.h"

//-----------------------------
bool urgWrapper::conn(string ip, int port)
{
	if (_isConn)
	{
		_urg.close();
	}

	if (!_urg.open(ip.c_str(), port, Lidar::Ethernet))
	{
		ofLog(OF_LOG_ERROR, "[urgWrapper::conn]Connection Failed");
		_isConn = false;
	}
	else
	{
		ofLog(OF_LOG_NOTICE, "[urgWrapper::conn]Connection Success");
		_isConn = true;
	}
	return _isConn;
	
}

//-----------------------------
void urgWrapper::draw(float ratio)
{
	if (!_isConn || !_isStart)
	{
		return;
	}

	ofVec2f v(0, 1);
	float unit = (float)abs(_endDeg - _startDeg) / _dataSize;
	float deg = _startDeg;
	ofVec2f o(0);

	ofPushStyle();
	ofSetColor(0, 255, 0);
	for (int i = 0; i < _dataSize; i++)
	{
		auto drawV = v.rotated(-deg);
		ofLine(o, o + drawV * _data[i] * ratio);
		deg += unit;
	}
	ofPopStyle();

}

//-----------------------------
void urgWrapper::start(int startDeg, int endDeg, int skip)
{
	if (!_isConn || _isStart)
	{
		return;
	}
	
	_timeStamp = 0;
	_urg.set_scanning_parameter(_urg.deg2step(startDeg), _urg.deg2step(endDeg), skip);
	_urg.start_measurement(Urg_driver::Distance);
	_dataSize = _urg.max_data_size();
	_data.resize(_dataSize);
	_startDeg = startDeg;
	_endDeg = endDeg;

	_isStart = true;
	_hasNewData = false;
	startThread(true, false);
}

//-----------------------------
void urgWrapper::stop()
{
	if (!_isConn || !_isStart)
	{
		return;
	}

	stopThread();
	_urg.stop_measurement();
	_isStart = false;
	ofLog(OF_LOG_NOTICE, "[urgWrapper::stop]Is stop");
}

//-----------------------------
vector<long>& urgWrapper::getData()
{
	_hasNewData = false;
	return _data;
}

int urgWrapper::getDataSize()
{
	return _dataSize;
}

//-----------------------------
bool urgWrapper::hasNewData()
{
	return _hasNewData;
}

//-----------------------------
void urgWrapper::enableSmooth(int size)
{
	_smoothSize = size;	
	_needSmooth = true;
}

//-----------------------------
void urgWrapper::disableSmooth()
{
	_needSmooth = false;
}

//-----------------------------
void urgWrapper::threadedFunction()
{
	if (!_isConn || !_isStart)
	{
		return;
	}
	
	while (isThreadRunning())
	{
		_dataSize = _urg.get_distanceFixSize(_data, &_timeStamp);
		if (_dataSize < 0)
		{
			ofLog(OF_LOG_ERROR, "[urgWrapper::update]Get distance failed");
		}
		if (_needSmooth)
		{
			smoothing();
		}

		_hasNewData = true;
	}
}

//--------------------------------------------------------------
void urgWrapper::smoothing()
{
	//Moving Average
	vector<long> tmp;
	tmp = _data;
	
	int kernalSize = _smoothSize * 2 + 1;
	for (int i = 0; i < _dataSize; i++)
	{
		int val = 0;
		for (int diff = -_smoothSize; diff <= _smoothSize; diff++)
		{
			auto index = i + diff;
			if (index < 0)
			{
				index = -index;
			}
			else if (index >= _dataSize)
			{
				index = tmp.size() - (index - _dataSize + 1) - 1;
			}
			val += tmp.at(index);
			
		}
		val /= (float)kernalSize;
		_data[i] = val;
	}
	
	
}

#pragma region Singleton
//--------------------------------------------------------------
urgWrapper* urgWrapper::pInstance = 0;
urgWrapper* urgWrapper::getInstance()
{
	if (pInstance == 0)
	{
		pInstance = new urgWrapper();
	}
	return pInstance;
}

//--------------------------------------------------------------
void urgWrapper::destroy()
{
	if (pInstance != 0)
	{
		delete pInstance;
	}
}
#pragma endregion
