#include "bloomFilter.h"

//-------------------------------------
void bloomFilter::init(int w, int h, bool arb)
{
	_bloom.init(w, h, arb);

	_bloom.createPass<BloomPass>()->setEnabled(false);
	_bloom.createPass<BloomPass>()->setEnabled(false);
	_bloom.setFlip(false);

}

//-------------------------------------
void bloomFilter::filterEnable()
{
	_bloom[0]->enable();
	_bloom[1]->enable();

}

//-------------------------------------
void bloomFilter::filterDisable()
{
	_bloom[0]->disable();
	_bloom[1]->disable();

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




