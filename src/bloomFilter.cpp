#include "bloomFilter.h"

//-------------------------------------
void bloomFilter::init(int w, int h, bool arb)
{
	_bloom.init(w, h, arb);

	_bloom.createPass<BloomPass>()->setEnabled(false);
	_bloom.createPass<BloomPass>()->setEnabled(false);
	_bloom.createPass<BloomPass>()->setEnabled(false);
	
	_bloom.setFlip(false);

}

//-------------------------------------
void bloomFilter::filterEnable()
{
	for (int i = 0; i < _bloom.size(); i++)
	{
		_bloom[i]->enable();
	}
}

//-------------------------------------
void bloomFilter::filterDisable()
{
	for (int i = 0; i < _bloom.size(); i++)
	{
		_bloom[i]->disable();
	}

}


#pragma region Singleton
//--------------------------------------------------------------
bloomFilter* bloomFilter::pInstance = 0;
bloomFilter* bloomFilter::GetInstance()
{
	if (pInstance == 0)
	{
		pInstance = new bloomFilter();
	}
	return pInstance;
}

//--------------------------------------------------------------
void bloomFilter::Destroy()
{
	if (pInstance != 0)
	{
		delete pInstance;
	}
}
#pragma endregion




