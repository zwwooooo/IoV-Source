

�ر�ע��������ļ�¼���������£��� source Ŀ¼��Դ���ļ�Ϊ׼��


/*************************************************************
IoVԴ���޸ķ�����¼        by zwwooooo - 2011.10.28

////////////////////////////////////
ʵ�ֵĹ���

��ı��daboboye=����

* ��<RangeBonus>��ԭ����ʵ�����Ӹ�Ϊ�������� by kenkenkenken
* �����ҹ����Ӿ��2/1���3/1 by zwwooooo
* ������� ItemSiz �ķŴ��ˡ� by kenkenkenken, edit zwwooooo
* �����ԭ�����������⣺by kenkenkenken, edit zwwooooo
	1. ���л�������AP
	2. ǹ����װ�� ��װ������AP ������ ��װ����AP
	3. ��һ���������񵯷���������ǹ�ϣ��ᰴװ��Ϊ������AP �����ǰ�װ������Ϊ������AP �� ���װһ��û�����񵯷���������ǹ�ϣ�AP�ǶԵ�
	4. �Ӳ�ǹ�ϲ�жһ���������񵯷�������AP�����bug���3�����ƣ�������һ�����������ؼ���ж���Ǹ����������� ����ǰ�� ���� ��������ǰ�棬����ǵ���ǰ���������bug
////////////////////////////////////


=========== ����ʵ�ֵĹ���˵��
----------- ����Ҫ�޸ĵ��ļ���
>>>>>>>>>>> Ҫ�޸ĵĴ���кţ��汾�������᲻ͬ���������ɣ�
\\\\\\\\\\\ ˵��ע��


= ��˵���ļ���¼�������ʱ ja2.exe Դ��汾 =
-svn4769 2011.10.28
-

�漰�޸ĵ��ļ���
Laptop\BobbyRGuns.cpp
Laptop\IMP Confirm.cpp

Tactical\Civ Quotes.cpp
Tactical\Civ Quotes.h
Tactical\Interface Enhanced.cpp
Tactical\Interface Items.cpp
Tactical\Item Types.cpp
Tactical\Items.cpp
Tactical\Items.h
Tactical\opplist.cpp
Tactical\Soldier Ani.cpp
Tactical\Weapons.cpp
Tactical\Weapons.h
Tactical\XML_LBEPocket.cpp

Utils\_ChineseText.cpp
Utils\_EnglishText.cpp
Utils\Text.h

gameloop.cpp
GameSettings.cpp
GameSetting.h
MainMenuScreen.cpp


**************************************************************/


===================================================================================================================

�ǰ�<RangeBonus>��ԭ����ʵ�����Ӹ�Ϊ��������

===================================================================================================================


------------------------------------------------------------------------------------------
Tactical\Weapons.cpp
------------------------------------------------------------------------------------------

>>>>>>>>>>>>>>��� line914 

		UINT16 usRange = GetModifiedGunRange(pObj->usItem);

		// Snap: attachment status is factored into the range bonus calculation
		rng = usRange + GetRangeBonus(pObj);

\\\\\\���ң�
rng = usRange + GetRangeBonus(pObj);
\\\\\\��Ϊ��
rng = ( Weapon[ pObj->usItem ].usRange * GetRangeBonusIoV(pObj) ) / 10000; //kenkenken: IoV921+z.4


------------------------------------------------------------------------------------------
Tactical\Items.cpp
------------------------------------------------------------------------------------------

>>>>>>>>>>>>>>��� line 9060 

\\\\\\�ҵ��������

INT16 GetRangeBonus( OBJECTTYPE * pObj )
{
	INT16 bonus = 0;
	if (pObj->exists() == true) {
		if(Item[pObj->usItem].usItemClass == IC_AMMO)
			return( Item[pObj->usItem].rangebonus );
		bonus = BonusReduce( Item[pObj->usItem].rangebonus, (*pObj)[0]->data.objectStatus );

		if ( (*pObj)[0]->data.gun.ubGunShotsLeft > 0 )
			bonus += Item[(*pObj)[0]->data.gun.usGunAmmoItem].rangebonus;

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if ( !Item[iter->usItem].duckbill || ( Item[iter->usItem].duckbill && (*pObj)[0]->data.gun.ubGunAmmoType == AMMO_BUCKSHOT ) && iter->exists())
				bonus += BonusReduce( Item[iter->usItem].rangebonus, (*iter)[0]->data.objectStatus );
		}
	}
	return( bonus );
}

\\\\\\����������� IoV ר�õ�<RangeBonus>���㹫ʽ����������

//kenkenkenken: IoV921+z.4
INT32 GetRangeBonusIoV( OBJECTTYPE * pObj )
{
	INT32 bonus = 10000;
	if (pObj->exists() == true) {
		bonus += ( BonusReduce( Item[pObj->usItem].rangebonus, (*pObj)[0]->data.objectStatus ) ) * 100;

		if ( (*pObj)[0]->data.gun.ubGunShotsLeft > 0 )
			bonus = ( bonus * ( 100 +  Item[(*pObj)[0]->data.gun.usGunAmmoItem].rangebonus ) ) / 100;

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if ( !Item[iter->usItem].duckbill || ( Item[iter->usItem].duckbill && (*pObj)[0]->data.gun.ubGunAmmoType == AMMO_BUCKSHOT ))
				bonus = ( bonus * ( 100 +  BonusReduce( Item[iter->usItem].rangebonus, (*iter)[0]->data.objectStatus ) ) ) / 100;
		}
	}
	return( bonus );
}
//<-- IoV


>>>>>>>>>>>>>>��� line��11373-19�� 

	// Read from item
	aimLevels = Weapon[pSoldier->inv[pSoldier->ubAttackingHand].usItem].ubAimLevels;
	fTwoHanded = Item[pSoldier->inv[pSoldier->ubAttackingHand].usItem].twohanded;
	weaponRange = Weapon[pSoldier->inv[pSoldier->ubAttackingHand].usItem].usRange + GetRangeBonus(&pSoldier->inv[pSoldier->ubAttackingHand]);
	weaponType = Weapon[pSoldier->inv[pSoldier->ubAttackingHand].usItem].ubWeaponType;
	fUsingBipod = FALSE;

\\\\\\�������
weaponRange = Weapon[pSoldier->inv[pSoldier->ubAttackingHand].usItem].usRange + GetRangeBonus(&pSoldier->inv[pSoldier->ubAttackingHand]);
\\\\\\��Ϊ
weaponRange = ( Weapon[pSoldier->inv[pSoldier->ubAttackingHand].usItem].usRange * GetRangeBonusIoV(&pSoldier->inv[pSoldier->ubAttackingHand]) ) / 10000; //kenkenkenken: IoV921+z.4


------------------------------------------------------------------------------------------
Tactical\Items.h
------------------------------------------------------------------------------------------

>>>>>>>>>>>>>>line 249
\\\\\\��
INT16 GetRangeBonus( OBJECTTYPE * pObj );
\\\\\\��������
INT32 GetRangeBonusIoV( OBJECTTYPE * pObj ); //kenkenkenken: IoV921+z.4



===================================================================================================================

ItemSize �������ֵΪ 54

===================================================================================================================

------------------------------------------------------------------------------------------
Tactical\Items.cpp
------------------------------------------------------------------------------------------

>>>>>>>>>>>>>>line 1421

	if (bSlot == STACK_SIZE_LIMIT) {
		//if it is stack size limit we want it to be a big slot or a vehicle slot
		if (UsingNewInventorySystem() == false)
			return (max(1, ubSlotLimit));
		else if(pSoldier != NULL && (pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE))
			return (max(1, LBEPocketType[VEHICLE_POCKET_TYPE].ItemCapacityPerSize[__min(34,Item[pObject->usItem].ItemSize)]));
		else
			return (max(1, min(255,LBEPocketType[VEHICLE_POCKET_TYPE].ItemCapacityPerSize[__min(34,Item[pObject->usItem].ItemSize)]*4)));
	}

\\\\\\�����34��Ϊ54

>>>>>>>>>>>>>>line 1467

	//We need to actually check the size of the largest stored item as well as the size of the current item
	if(cntAttach == TRUE)
	{
		iSize = CalculateItemSize(pObject);
		if(pSoldier != NULL && pSoldier->inv[bSlot].usItem == pObject->usItem)
		{
			sSize = CalculateItemSize(&pSoldier->inv[bSlot]);
			if(LBEPocketType[pIndex].ItemCapacityPerSize[sSize] < LBEPocketType[pIndex].ItemCapacityPerSize[iSize])
				iSize = sSize;
		}
	}
	else
		iSize = Item[pObject->usItem].ItemSize;
	iSize = __min(iSize,54);
	ubSlotLimit = LBEPocketType[pIndex].ItemCapacityPerSize[iSize];

	//this could be changed, we know guns are physically able to stack
	//if ( iSize < 10 && ubSlotLimit > 1)
	//	ubSlotLimit = 1;

\\\\\\34��Ϊ54

>>>>>>>>>>>>>>line 2504

#if 0
//old method
				UINT16	newSize, testSize, maxSize;
				UINT8	cnt=0;
				newSize = 0;
				maxSize = max(iSize, LoadBearingEquipment[Item[pObject->usItem].ubClassIndex].lbeFilledSize);
				// Look for the ItemSize of the largest item in this LBENODE
				for(unsigned int x = 0; x < pLBE->inv.size(); ++x)
				{
					if(pLBE->inv[x].exists() == true)
					{
						testSize = CalculateItemSize(&(pLBE->inv[x]));
						//Now that we have the size of one item, we want to factor in the number of items since two
						//	items take up more space then one.
						testSize = testSize + pLBE->inv[x].ubNumberOfObjects - 1;
						testSize = min(testSize,54); // zwwooooo: IoV921+z.4 = 54, 1.13 = 34
						//We also need to increase the size of guns so they'll fit with the rest of our calculations.
						if(testSize < 5)
							testSize += 10;
						if(testSize < 10)
							testSize += 18;
						//Finally, we want to factor in multiple pockets.  We'll do this by counting the number of filled
						//	pockets, then add this count total to our newSize when everything is finished.
						cnt++;
						newSize = max(testSize, newSize);
					}
				}
				//Add the total number of filled pockets to our NewSize to account for multiple pockets being used
				newSize += cnt;
				newSize = min(newSize,54); // zwwooooo: IoV921+z.4 = 54, 1.13 = 34
				// If largest item is smaller then LBE, don't change ItemSize
				if(newSize > 0 && newSize < iSize) {
					iSize = iSize;
				}
				// if largest item is larget then LBE but smaller then max size, partially increase ItemSize
				else if(newSize >= iSize && newSize < maxSize) {
					iSize = newSize;
				}
				// if largest item is larger then max size, reset ItemSize to max size
				else if(newSize >= maxSize) {
					iSize = maxSize;
				}
#endif


>>>>>>>>>>>>>>line 2725

// CHRISL: New function to dynamically modify ItemSize based on attachments, stack size, etc
UINT16 CalculateItemSize( OBJECTTYPE *pObject )
{
	UINT16		iSize;
	UINT16		currentSize = 0;
	UINT32		cisIndex;

	// Determine default ItemSize based on item and attachments
	cisIndex = pObject->usItem;
	iSize = Item[cisIndex].ItemSize;
	if(iSize>34)
		iSize = 34;

	//for each object in the stack, hopefully there is only 1
	for (int numStacked = 0; numStacked < pObject->ubNumberOfObjects; ++numStacked) {
		//some weapon attachments can adjust the ItemSize of a weapon
		if(iSize<10) {
			for (attachmentList::iterator iter = (*pObject)[numStacked]->attachments.begin(); iter != (*pObject)[numStacked]->attachments.end(); ++iter) {
				if (iter->exists() == true) {
					iSize += Item[iter->usItem].itemsizebonus;
					// CHRISL: This is to catch things if we try and reduce ItemSize when we're already at 0
					if(iSize > 34 || iSize < 0)
						iSize = 0;
					if(iSize > 9)
						iSize = 9;
				}
			}
		}

\\\\\\34��Ϊ54


---------------------------------------------
IoV ��Ʒ����
���ͣ�ԭ���0��9��������10�ǳ�С��Ʒ����11��ʼ��34��ÿ4�����ִ���һ����񣬲�ͬ���ִ���ͬ��״����6��4��

�޸�Ϊ��0��12��������12�ǳ�С��Ʒ����13��54��ÿ7�����ִ���һ����񣬲�ͬ���ִ���ͬ��״����6��7�� ��by zwwooooo��
---------------------------------------------

>>>>>>>>>>>>>>line 2735(�޸ĺ�)

	//for each object in the stack, hopefully there is only 1
	for (int numStacked = 0; numStacked < pObject->ubNumberOfObjects; ++numStacked) {
		//some weapon attachments can adjust the ItemSize of a weapon
		if(iSize<12) { //kenkenkenken: IoV921+z.4
			for (attachmentList::iterator iter = (*pObject)[numStacked]->attachments.begin(); iter != (*pObject)[numStacked]->attachments.end(); ++iter) {
				if (iter->exists() == true) {
					iSize += Item[iter->usItem].itemsizebonus;
					// CHRISL: This is to catch things if we try and reduce ItemSize when we're already at 0
					if(iSize > 54 || iSize < 0) //kenkenkenken: IoV921+z.4 = 54 1.13 = 34
						iSize = 0;
					if(iSize > 11) //kenkenkenken: IoV921+z.4 = 11 1.13 = 9
						iSize = 11; //kenkenkenken: IoV921+z.4 = 11 1.13 = 9
				}
			}
		}

\\\\\\�޸Ĺ�����ֵ�� IoV ��ʶ��˵��


>>>>>>>>>>>>>>line 2683���޸ĺ�

\\\\\\�ҵ��˺���

int GetPocketSizeByDimensions(int sizeX, int sizeY)
{
	static const UINT8 cisPocketSize[6][7] = //zwwooooo: IoV921+z.4 = [6][7], 1.13 = [6][4]
	{
		//11, 12, 13, 14,
		//15, 16, 17, 18,
		//19, 20, 21, 22,
		//23, 24, 25, 26,
		//27, 28, 29, 30,
		//31, 32, 33, 34
		13, 14, 15, 16, 17, 18, 19, //IoV921+z.4
		20, 21, 22, 23, 24, 25, 26,
		27, 28, 29, 30, 31, 32, 33,
		34, 35, 36, 37, 38, 39, 40,
		41, 42, 43, 44, 45, 46, 47,
		48, 49, 50, 51, 52, 53, 54
	};
	return cisPocketSize[sizeX][sizeY];
}

void GetPocketDimensionsBySize(int pocketSize, int& sizeX, int& sizeY)
{
	static const UINT8 cisPocketSize[6][7] = //zwwooooo: IoV921+z.4 = [6][7], 1.13 = [6][4]
	{
		//11, 12, 13, 14,
		//15, 16, 17, 18,
		//19, 20, 21, 22,
		//23, 24, 25, 26,
		//27, 28, 29, 30,
		//31, 32, 33, 34
		13, 14, 15, 16, 17, 18, 19, // IoV 921-001
		20, 21, 22, 23, 24, 25, 26,
		27, 28, 29, 30, 31, 32, 33,
		34, 35, 36, 37, 38, 39, 40,
		41, 42, 43, 44, 45, 46, 47,
		48, 49, 50, 51, 52, 53, 54
	};

	for(sizeX=0; sizeX<6; sizeX++)
	{
		for(sizeY=0; sizeY<7; sizeY++) //zwwooooo: IoV921+z.4 sizeY<7, 1.13 sizeY<4
		{
			if(pocketSize == cisPocketSize[sizeX][sizeY])
			{
				return;
			}
		}
	}
}

\\\\\\ע�͵����� 1.13 ����ֵ, �޸Ĺ�����ֵ����� IoV


------------------------------------------------------------------------------------------
Tactical\Item Types.cpp
------------------------------------------------------------------------------------------

>>>>>>>>>>>>>>line 417

POCKETTYPE::POCKETTYPE(){
	memset(this, 0, SIZEOF_POCKETTYPE);
	ItemCapacityPerSize.resize(55); //kenkenkenken: IoV921+z.4 =55, 1.13 = 35
}

\\\\\\���������Ʒ�����54����ô�� 54+1


>>>>>>>>>>>>>>line 421

POCKETTYPE::POCKETTYPE(const POCKETTYPE& src){
	memcpy(this, &src, SIZEOF_POCKETTYPE);
	ItemCapacityPerSize.resize(55); //kenkenkenken: IoV921+z.4 =55, 1.13 = 35
	ItemCapacityPerSize = src.ItemCapacityPerSize;
}

\\\\\\���������Ʒ�����54����ô�� 54+1

------------------------------------------------------------------------------------------
Tactical\XML_LBEPocket.cpp
------------------------------------------------------------------------------------------

>>>>>>>>>>>>>>line 92

\\\\\\�ҵ�
strcmp(name, "ItemCapacityPerSize.34") == 0 || strcmp(name, "ItemCapacityPerSize34") == 0 ))
\\\\\\��Ϊ��

				strcmp(name, "ItemCapacityPerSize.34") == 0 || strcmp(name, "ItemCapacityPerSize34") == 0 ||
				strcmp(name, "ItemCapacityPerSize.35") == 0 || strcmp(name, "ItemCapacityPerSize35") == 0 ||
				strcmp(name, "ItemCapacityPerSize.36") == 0 || strcmp(name, "ItemCapacityPerSize36") == 0 ||
				strcmp(name, "ItemCapacityPerSize.37") == 0 || strcmp(name, "ItemCapacityPerSize37") == 0 ||
				strcmp(name, "ItemCapacityPerSize.38") == 0 || strcmp(name, "ItemCapacityPerSize38") == 0 ||
				strcmp(name, "ItemCapacityPerSize.39") == 0 || strcmp(name, "ItemCapacityPerSize39") == 0 ||
				strcmp(name, "ItemCapacityPerSize.40") == 0 || strcmp(name, "ItemCapacityPerSize40") == 0 ||
				strcmp(name, "ItemCapacityPerSize.41") == 0 || strcmp(name, "ItemCapacityPerSize41") == 0 ||
				strcmp(name, "ItemCapacityPerSize.42") == 0 || strcmp(name, "ItemCapacityPerSize42") == 0 ||
				strcmp(name, "ItemCapacityPerSize.43") == 0 || strcmp(name, "ItemCapacityPerSize43") == 0 ||
				strcmp(name, "ItemCapacityPerSize.44") == 0 || strcmp(name, "ItemCapacityPerSize44") == 0 ||
				strcmp(name, "ItemCapacityPerSize.45") == 0 || strcmp(name, "ItemCapacityPerSize45") == 0 ||
				strcmp(name, "ItemCapacityPerSize.46") == 0 || strcmp(name, "ItemCapacityPerSize46") == 0 ||
				strcmp(name, "ItemCapacityPerSize.47") == 0 || strcmp(name, "ItemCapacityPerSize47") == 0 ||
				strcmp(name, "ItemCapacityPerSize.48") == 0 || strcmp(name, "ItemCapacityPerSize48") == 0 ||
				strcmp(name, "ItemCapacityPerSize.49") == 0 || strcmp(name, "ItemCapacityPerSize49") == 0 ||
				strcmp(name, "ItemCapacityPerSize.50") == 0 || strcmp(name, "ItemCapacityPerSize50") == 0 ||
				strcmp(name, "ItemCapacityPerSize.51") == 0 || strcmp(name, "ItemCapacityPerSize51") == 0 ||
				strcmp(name, "ItemCapacityPerSize.52") == 0 || strcmp(name, "ItemCapacityPerSize52") == 0 ||
				strcmp(name, "ItemCapacityPerSize.53") == 0 || strcmp(name, "ItemCapacityPerSize53") == 0 ||
				strcmp(name, "ItemCapacityPerSize.54") == 0 || strcmp(name, "ItemCapacityPerSize54") == 0 ))

\\\\\\�ҵ�
		else if(strcmp(name, "ItemCapacityPerSize.34") == 0 || strcmp(name, "ItemCapacityPerSize34") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[34] = (UINT8) atol(pData->szCharData);
		}
\\\\\\�����������ӣ�
		//kenkenkenken: IoV921+z.4 add -->
		else if(strcmp(name, "ItemCapacityPerSize.35") == 0 || strcmp(name, "ItemCapacityPerSize35") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[35] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.36") == 0 || strcmp(name, "ItemCapacityPerSize36") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[36] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.37") == 0 || strcmp(name, "ItemCapacityPerSize37") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[37] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.38") == 0 || strcmp(name, "ItemCapacityPerSize38") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[38] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.39") == 0 || strcmp(name, "ItemCapacityPerSize39") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[39] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.40") == 0 || strcmp(name, "ItemCapacityPerSize40") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[40] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.41") == 0 || strcmp(name, "ItemCapacityPerSize41") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[41] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.42") == 0 || strcmp(name, "ItemCapacityPerSize42") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[42] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.43") == 0 || strcmp(name, "ItemCapacityPerSize43") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[43] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.44") == 0 || strcmp(name, "ItemCapacityPerSize44") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[44] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.45") == 0 || strcmp(name, "ItemCapacityPerSize45") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[45] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.46") == 0 || strcmp(name, "ItemCapacityPerSize46") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[46] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.47") == 0 || strcmp(name, "ItemCapacityPerSize47") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[47] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.48") == 0 || strcmp(name, "ItemCapacityPerSize48") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[48] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.49") == 0 || strcmp(name, "ItemCapacityPerSize49") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[49] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.50") == 0 || strcmp(name, "ItemCapacityPerSize50") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[50] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.51") == 0 || strcmp(name, "ItemCapacityPerSize51") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[51] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.52") == 0 || strcmp(name, "ItemCapacityPerSize52") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[52] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.53") == 0 || strcmp(name, "ItemCapacityPerSize53") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[53] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.54") == 0 || strcmp(name, "ItemCapacityPerSize54") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[54] = (UINT8) atol(pData->szCharData);
		}
		// IoV <--



\\\\\\�ҵ�
FilePrintf(hFile,"\t\t<ItemCapacityPerSize34>%d</ItemCapacityPerSize34>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[34]   );
\\\\\\�����������ӣ�

			FilePrintf(hFile,"\t\t<ItemCapacityPerSize35>%d</ItemCapacityPerSize35>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[35]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize36>%d</ItemCapacityPerSize36>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[36]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize37>%d</ItemCapacityPerSize37>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[37]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize38>%d</ItemCapacityPerSize38>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[38]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize39>%d</ItemCapacityPerSize39>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[39]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize40>%d</ItemCapacityPerSize40>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[40]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize41>%d</ItemCapacityPerSize41>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[41]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize42>%d</ItemCapacityPerSize42>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[42]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize43>%d</ItemCapacityPerSize43>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[43]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize44>%d</ItemCapacityPerSize44>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[44]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize45>%d</ItemCapacityPerSize45>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[45]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize46>%d</ItemCapacityPerSize46>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[46]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize47>%d</ItemCapacityPerSize47>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[47]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize48>%d</ItemCapacityPerSize48>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[48]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize49>%d</ItemCapacityPerSize49>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[49]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize50>%d</ItemCapacityPerSize50>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[50]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize51>%d</ItemCapacityPerSize51>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[51]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize52>%d</ItemCapacityPerSize52>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[52]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize53>%d</ItemCapacityPerSize53>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[53]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize54>%d</ItemCapacityPerSize54>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[54]   );

------------------------------------------------------------------------------------------
/Laptop/IMP Confirm.cpp ����Ҫ������˴�û���޸ģ���ôIMP/AIM����ʱ����34����Ʒ�޷�Я���� by zwwooooo
------------------------------------------------------------------------------------------

>>>>>>>>>>>>>>line 513

	// Next sort list by size
	for(j=54; j>=0; j--) //zwwooooo: IoV921+z.4 =54, 1.13 = 34 - ItemSize limit
	{
		for(i=0; i<length; i++)
		{
			if(tInv[i].iSize == j)
			{
				//ADB this is a mem leak!!!
				//int *filler = new int;
				//iOrder.push_back(*filler);
				iOrder[count] = i;
				count++;
			}
		}
	}



===================================================================================================================

�Ӿ��޸� by zwwooooo

===================================================================================================================

------------------------------------------------------------------------------------
Tactical\opplist.cpp
------------------------------------------------------------------------------------

>>>>>>>>>>>>>>line 239

\\\\\\�ҵ�����

// % values of sighting distance at various light levels

INT8 gbLightSighting[1][SHADE_MIN+1] =
{
{ // human
	80, // brightest
	86,
	93,
	100, // normal daylight, 3
	94,
	88,
	82,
	76,
	70, // mid-dawn, 8
	64,
	58,
	51,
	43, // normal nighttime, 12 (11 tiles)
	30,
	17,
	9
}
};

\\\\\\��Ϊ:

// % values of sighting distance at various light levels

INT8 gbLightSighting[1][SHADE_MIN+1] =
{
{ // human
	80, // brightest
	86,
	93,
	100, // normal daylight, 3
	94,
	86, //88,
	79, //82,
	72, //76,
	65, //70, // mid-dawn, 8
	57, //64,
	49, //58,
	41, //51,
	33, //43, // normal nighttime, 12 (11 tiles)
	25, //30,
	17,
	9
}
};

===================================================================================================================

kenkenkenken �޸� http://tbsgame.net/bbs/index.php?showtopic=58400&view=findpost&p=813110

-JA2���񵯻�ǹ����񵯶�������AP��bug,

����ͨ���޸�Tactical\Interface Items.cpp" from line 4694 to 4741.ʵ��

(bp�ϵ�silversurfer����˵); 

TV��

����������Ѿ��޸��ˣ��������д��һ�£������ԭ�����������⣺

1. ���л�������AP

2. ǹ����װ�� ��װ������AP ������ ��װ����AP

3. ��һ���������񵯷���������ǹ�ϣ��ᰴװ��Ϊ������AP �����ǰ�װ������Ϊ������AP �� ���װһ��û�����񵯷���������ǹ�ϣ�AP�ǶԵ�

4. �Ӳ�ǹ�ϲ�жһ���������񵯷�������AP�����bug���3�����ƣ�������һ�����������ؼ���ж���Ǹ����������� ����ǰ�� ���� ��������ǰ�棬����ǵ���ǰ���������bug

���루���������ε��ϴ��룩���£�

- 2011.10.28 ���������������°汾4769��zwwooooo ע��

===================================================================================================================


	// check for any AP costs
	if ( ( gTacticalStatus.uiFlags & TURNBASED ) && ( gTacticalStatus.uiFlags & INCOMBAT ) )
	{
		if (gpAttachSoldier)
		{
			UINT8 ubAPCost = 0;

			// check for change in attachments
			//unsigned int originalSize = gOriginalAttachments.size();
			//unsigned int newSize = (*gpItemDescObject)[0]->attachments.size();
			
			//WarmSteel size() is no longer sufficient, because the list contains null objects.
			unsigned int originalSize = 0;
			unsigned int newSize = 0;
/*			attachmentList::iterator originalIter;
			attachmentList::iterator newIter;

			for (originalIter = gOriginalAttachments.begin(), newIter = (*gpItemDescObject)[0]->attachments.begin();
			originalIter != gOriginalAttachments.end() && newIter != (*gpItemDescObject)[0]->attachments.end();
			++originalIter, ++newIter){
				if(originalIter->exists()){
					originalSize++;
				}
				if(newIter->exists()){
					newSize++;
				}
			}


			if (newSize != originalSize) {
				//an attachment was changed, find the change
				for (originalIter = gOriginalAttachments.begin(), newIter = (*gpItemDescObject)[0]->attachments.begin();
					originalIter != gOriginalAttachments.end() && newIter != (*gpItemDescObject)[0]->attachments.end();
					++originalIter, ++newIter) {
					if (*originalIter == *newIter) {
						continue;
					}
					else {
						break;
					}
				}
				if (newSize < originalSize) {
					//an attachment was removed, charge APs
					ubAPCost = AttachmentAPCost(originalIter->usItem,gpItemDescObject, gpAttachSoldier ); // SANDRO - added argument
				}
				else {
					//an attachment was added charge APs
					//lalien: changed to charge AP's for reloading a GL/RL
					if ( Item[ gpItemDescObject->usItem ].usItemClass == IC_LAUNCHER || Item[gpItemDescObject->usItem].cannon )
					{
						ubAPCost = GetAPsToReload( gpItemDescObject );
					}
					else
					{
						ubAPCost = AttachmentAPCost(newIter->usItem,gpItemDescObject, gpAttachSoldier); // SANDRO - added argument
					}
				}
			}
*/
			//kenkenkenken: IoV921+z.4 ---begin
			attachmentList::iterator originalIter;
			attachmentList::iterator newIter;
     
			BOOLEAN nextLoop = FALSE;  
			BOOLEAN launcherSearch;

			// TRUE means the attachment we are dealing with is a loaded launcher
			abs ( int ( originalSize - newSize ) ) == 2 ? launcherSearch = TRUE : launcherSearch = FALSE;

			if (newSize < originalSize) {    // An attachment was removed    

				for (originalIter = gOriginalAttachments.begin(); originalIter != gOriginalAttachments.end(); ++originalIter) {
   
					nextLoop = FALSE;
   
					for (newIter = (*gpItemDescObject)[0]->attachments.begin(); newIter != (*gpItemDescObject)[0]->attachments.end(); ++newIter) {
						if (*originalIter == *newIter){
							nextLoop = TRUE;
							break;      
						}
						else {
							continue;
						}
					}

					if (!nextLoop) {
						if (launcherSearch && Item[ originalIter->usItem ].usItemClass != IC_LAUNCHER){     // We are looking for that launcher, not the grenade, so continue
							continue;      
						}
						else {
							break;
						}      
					}
					else {
						continue;
					}
				}

				ubAPCost = AttachmentAPCost(originalIter->usItem,gpItemDescObject, gpAttachSoldier);

			}
			else { // Other situations: attachment adding, grenade switching, or nothing changed
 
				for (newIter = (*gpItemDescObject)[0]->attachments.begin(); newIter != (*gpItemDescObject)[0]->attachments.end(); ++newIter) {
	   
					nextLoop = FALSE;
		   
					for (originalIter = gOriginalAttachments.begin(); originalIter != gOriginalAttachments.end(); ++originalIter) {
						if (*newIter == *originalIter){
							nextLoop = TRUE;
							break;      
						}
						else {
							continue;
						}
					}

					if (!nextLoop) {
						if (launcherSearch && Item[ newIter->usItem ].usItemClass != IC_LAUNCHER){ // We are looking for that launcher, not the grenade, so continue
							continue;      
						}
						else {
							if ( Item[newIter->usItem].glgrenade ) {
								if ( Item[ gpItemDescObject->usItem ].usItemClass == IC_LAUNCHER ) { // Standalone GL reloading or grenade switching
									ubAPCost = GetAPsToReload( gpItemDescObject ); // AP cost = reloading APs of this standalone GL
								}
								else { // Rifle-attached GL reloading or grenade switching
									for (originalIter = gOriginalAttachments.begin(); originalIter != gOriginalAttachments.end(); ++originalIter) {
										if ( Item[ originalIter->usItem ].usItemClass == IC_LAUNCHER ) {
											break;
										}
										else {
											continue;
										}
									}
									ubAPCost = Weapon[ originalIter->usItem ].APsToReload; // AP cost = reloading APs of this attached GL
								}
							}
							else {
								ubAPCost = AttachmentAPCost(newIter->usItem,gpItemDescObject, gpAttachSoldier); // Normal attachment adding
							}
							break;
						}      
					}
					else {
						continue;
					}
				}
			}  
			//IoV---end

			if (ubAPCost)
			{
				DeductPoints( gpAttachSoldier, ubAPCost, 0 );
			}
		}

===================================================================================================================

zwwooooo: IoV921+ : ���ֽ�У������ࣩ���ܵ�������

===================================================================================================================

------------------------------------------------------------------------------------
Tactical\Item.cpp
------------------------------------------------------------------------------------

>>>>>>>>>>>>>>line 5963

		// CHRISL: When holding ammo and clicking on an appropriate ammo crate, add ammo to crate
		if(Item[pInSlot->usItem].ammocrate == TRUE && Item[pObj->usItem].usItemClass == IC_AMMO)
		{
			if(Magazine[Item[pInSlot->usItem].ubClassIndex].ubCalibre == Magazine[Item[pObj->usItem].ubClassIndex].ubCalibre &&
				Magazine[Item[pInSlot->usItem].ubClassIndex].ubAmmoType == Magazine[Item[pObj->usItem].ubClassIndex].ubAmmoType)
			{
				UINT16	magSpace = Magazine[Item[pInSlot->usItem].ubClassIndex].ubMagSize-(*pInSlot)[0]->data.ubShotsLeft;
				while(pObj->ubNumberOfObjects > 0)
				{
					if(magSpace >= (*pObj)[0]->data.ubShotsLeft)
					{
						magSpace -= (*pObj)[0]->data.ubShotsLeft;
						(*pInSlot)[0]->data.ubShotsLeft += (*pObj)[0]->data.ubShotsLeft;
						pObj->RemoveObjectsFromStack(1);
					}
					else
					{
						(*pObj)[0]->data.ubShotsLeft -= magSpace;
						(*pInSlot)[0]->data.ubShotsLeft += magSpace;
						break;
					}
				}
				// if(pObj->ubNumberOfObjects > 0)
					// return( FALSE );
				// else
					// return( TRUE );
				if(pObj->ubNumberOfObjects <= 0) return( TRUE ); // zwwooooo: IoV921+z.4 : ���ֽ�У������ࣩ���ܵ�������
			}
		}

		

===================================================================================================================

�µĹ������㷨 by kenkenkenken

===================================================================================================================

------------------------------------------------------------------------------------
GameSetting.h
------------------------------------------------------------------------------------

>>>>>>>>>>>>>>line 974

	//dnl ch51 081009 JA2 Debug Settings
	BOOLEAN fEnableInventoryPoolQ;		
	
	//kenkenkenken: IoV921+z.5 -->
	UINT8 iMalfunctionRateDivisorBasic;
	UINT8 iMalfunctionRateDivisorBurst;
    //<-- IoV

	//legion by Jazz
	//BOOLEAN fShowTacticalFaceGear; //legion 2
	//BOOLEAN fShowTacticalFaceIcons; //legion 2
	INT8 bTacticalFaceIconStyle;


>>>>>>>>>>>>>>line 1373

// Snap: Read options from an INI file in the default of custom Data directory
void LoadGameExternalOptions();
void LoadSkillTraitsExternalSettings(); // SANDRO - added this one
void LoadIoVSettings(); //kenkenkenken: IoV921+z.5

------------------------------------------------------------------------------------
gameloop.cpp
------------------------------------------------------------------------------------

>>>>>>>>>>>>>>line 127

	// Load new ini - SANDRO
	LoadSkillTraitsExternalSettings();
	LoadIoVSettings(); //kenkenkenken: IoV921+z.5

	// HEADROCK HAM 4: Read CTH values
	LoadCTHConstants();

------------------------------------------------------------------------------------
GameSettings.cpp
------------------------------------------------------------------------------------

>>>>>>>>>>>>>>line 55

#define				AP_BP_CONSTANTS_FILE			"APBPConstants.ini"

#define				STOMP_SETTINGS_FILE				"Skills_Settings.ini" // SANDRO - file for STOMP

#define				IOV_SETTINGS_FILE	    		"IoV_Settings.ini" //kenkenkenken: IoV921+z.5

// HEADROCK HAM 4: This file contains all the settings required to tweak the new Shooting Mechanism. There's lots of them.


>>>>>>>>>>>>>>line 1929

\\\\\\\\�����溯��
INT16 DynamicAdjustAPConstants(INT16 iniReadValue, INT16 iniDefaultValue, BOOLEAN reverse)	
\\\\\\\\ǰ����� IoV ���ú���
//kenkenken: IoV921+z.5 -->
void LoadIoVSettings()
{
	CIniReader iniReader(IOV_SETTINGS_FILE);

	gGameExternalOptions.ubStraightSightRange		  = iniReader.ReadInteger("Tactical Vision Settings","BASE_SIGHT_RANGE",15, 5, 100);
	gGameExternalOptions.iMalfunctionRateDivisorBasic = iniReader.ReadInteger("Malfunction Settings","MALFUNCTION_RATE_DIVISOR_BASIC",100, 1, 255);
	gGameExternalOptions.iMalfunctionRateDivisorBurst = iniReader.ReadInteger("Malfunction Settings","MALFUNCTION_RATE_DIVISOR_BURST",125, 1, 255);
}
//<-- IoV

------------------------------------------------------------------------------------
MainMenuScreen.cpp
------------------------------------------------------------------------------------

>>>>>>>>>>>>>>line 413

	// Load new STOMP ini - SANDRO
	LoadSkillTraitsExternalSettings();
	LoadIoVSettings(); //kenkenkenken: IoV921+z.5
	// HEADROCK HAM 4: CTH constants
	LoadCTHConstants();

	
------------------------------------------------------------------------------------
\Laptop\BobbyRGuns.cpp
------------------------------------------------------------------------------------

>>>>>>>>>>>>>>line 2580

				//kenkenkenken: IoV921+z.5 -->
				UINT8 mrStrNo;
				UINT16 mrValue = Weapon[ pItemNumbers[ i ] ].MalfunctionRate;
				if (mrValue == 0)
					mrValue = 255;				
				if( mrValue >= 255)
				{
					mrStrNo = 19;
				}
				else if( mrValue < 255 && mrValue >= 225)
				{
					mrStrNo = 20;
				}
				else if( mrValue < 225 && mrValue >= 200)
				{
					mrStrNo = 21;
				}
				else if( mrValue < 200 && mrValue >= 175)
				{
					mrStrNo = 22;
				}
				else if( mrValue < 175 && mrValue >= 150)
				{
					mrStrNo = 23;
				}
				else if( mrValue < 150 && mrValue >= 125)
				{
					mrStrNo = 24;
				}
				else if( mrValue < 125 && mrValue >= 100)
				{
					mrStrNo = 25;
				}
				else if( mrValue < 100 && mrValue >= 75)
				{
					mrStrNo = 26;
				}
				else if( mrValue < 75 && mrValue >= 50)
				{
					mrStrNo = 27;
				}
				else if( mrValue < 50)
				{
					mrStrNo = 28;
				}
				//<-- IoV

				// HEADROCK HAM 3: Added last string (attachStr), for display of the possible attachment list. 
				// If the feature is deactivated, the attachStr will simply be empty at this point 
				// (remember? we emptied it earlier!).
				INT8 accuracy = (UsingNewCTHSystem()==true?Weapon[ pItemNumbers[ i ] ].nAccuracy:Weapon[ pItemNumbers[ i ] ].bAccuracy);
				//swprintf( pStr, L"%s (%s)\n%s %d\n%s %d\n%s %d\n%s (%d) %s\n%s %1.1f %s%s",
				swprintf( pStr, L"%s (%s)\n%s %d\n%s %d\n%s %d\n%s (%d) %s\n%s %1.1f %s\n%s %s%s", //zwwoooooo: IoV921+z.5
					ItemNames[ pItemNumbers[ i ] ],
					AmmoCaliber[ Weapon[ pItemNumbers[ i ] ].ubCalibre ],
					gWeaponStatsDesc[ 9 ],					//Accuracy String
					accuracy,								//Accuracy
					gWeaponStatsDesc[ 11 ],					//Damage String
					gunDamage,								//Gun damage
					gWeaponStatsDesc[ 10 ],					//Range String
					gGameSettings.fOptions[ TOPTION_SHOW_WEAPON_RANGE_IN_TILES ] ? gunRange/10 : gunRange,	//Gun Range 
					gWeaponStatsDesc[ 6 ],					//AP String
					readyAPs,
					apStr,									//AP's
					//L"- / - / -",
					gWeaponStatsDesc[ 12 ],					//Weight String
					fWeight,								//Weight
					GetWeightUnitString(),					//Weight units
					//zwwooooo: IoV921+z.5 -->					
					gWeaponStatsDesc[ 18 ], //Malfunction rate String
					gWeaponStatsDesc[ mrStrNo ], //Malfunction rate
					//<-- IoV
					attachStr
					);
			}
			break;

	
------------------------------------------------------------------------------------
\Tactical\Interface Enhanced.cpp
------------------------------------------------------------------------------------

>>>>>>>>>>>>>>line 5474
\\\\\\\\��
		/////////////////// MinRangeForAimBonus
		if( UsingNewCTHSystem() == false )
\\\\\\\\��������������һ����ʾģ�����NCTH�����²���ʾ��MinRangeForAimBonus

		//zwwooooo: IoV921+z.5 ������ EDB ��ʾ���� : ��ʾλ���ڡ�REPAIR EASE������ -->
		if( UsingNewCTHSystem() == true ){ //ֻ�ڿ���NCTH�Ź�����ע����Ϊ��ʾλ�����⣬ȡ��δ����NCTH����ʾ��
			/////////////////// MalfunctionRate
			{
				// Set line to draw into
				ubNumLine = 10;
				// Set Y coordinates
				sTop = gItemDescGenRegions[ubNumLine][1].sTop;
				sHeight = gItemDescGenRegions[ubNumLine][1].sBottom - sTop;

				// Get MalfunctionRate ��ȡ���������ݣ��㷨 by kenkenkenken
				UINT8 mrStrNo;
				UINT16 mrValue = Weapon[gpItemDescObject->usItem].MalfunctionRate;
				if (mrValue == 0)
					mrValue = 255;
				mrValue = mrValue * 100;
				if( mrValue >= 25500)
				{
					mrStrNo = 19;
				}
				else if( mrValue < 25500 && mrValue >= 22500)
				{
					mrStrNo = 20;
				}
				else if( mrValue < 22500 && mrValue >= 20000)
				{
					mrStrNo = 21;
				}
				else if( mrValue < 20000 && mrValue >= 17500)
				{
					mrStrNo = 22;
				}
				else if( mrValue < 17500 && mrValue >= 15000)
				{
					mrStrNo = 23;
				}
				else if( mrValue < 15000 && mrValue >= 12500)
				{
					mrStrNo = 24;
				}
				else if( mrValue < 12500 && mrValue >= 10000)
				{
					mrStrNo = 25;
				}
				else if( mrValue < 10000 && mrValue >= 7500)
				{
					mrStrNo = 26;
				}
				else if( mrValue < 7500 && mrValue >= 5000)
				{
					mrStrNo = 27;
				}
				else if( mrValue < 5000)
				{
					mrStrNo = 28;
				}

				// Print rate String
				SetFontForeground( FONT_MCOLOR_WHITE );
				sLeft = gItemDescGenRegions[ubNumLine][1].sLeft - 52;
				sWidth = 50; //gItemDescGenRegions[ubNumLine][1].sRight - sLeft;
				swprintf( pStr, L"%s", gWeaponStatsDesc[ 18 ] ); //Malfunction rate String
				FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				mprintf( usX, usY, pStr );

				// Print final value
				SetFontForeground( 5 );
				swprintf( pStr, L"%s", gWeaponStatsDesc[ mrStrNo ] ); //Malfunction rate
				mprintf( usX+50, usY, pStr );
			}
		}
		// <-- IoV
					
------------------------------------------------------------------------------------
Tactical\Weapons.cpp
------------------------------------------------------------------------------------

>>>>>>>>>>>>>>line 35

	#include "Soldier Functions.h" // added by SANDRO
	#include "Drugs And Alcohol.h" // HEADROCK HAM 4: Get drunk level
	#include "LOS.h" // HEADROCK HAM 4: Required for new shooting mechanism. Alternately, maybe move the functions to LOS.h.
    #include "civ quotes.h" //kenkenkenken: IoV921+z.5
#endif


>>>>>>>>>>>>>>line 421

				strcmp(name, "APsToReloadManually") == 0 ||
				strcmp(name, "ManualReloadSound") == 0 ||
				strcmp(name, "MalfunctionRate") == 0 || //kenkenkenken: IoV921+z.5
				strcmp(name, "ubAimLevels") == 0  ||// HEADROCK HAM 4: Allowed aiming levels for this gun.
				strcmp(name, "bRecoilX") == 0 || // HEADROCK HAM 4:
				strcmp(name, "bRecoilY") == 0 || // HEADROCK HAM 4:
				strcmp(name, "ubRecoilDelay") == 0 || // HEADROCK HAM 4:
				strcmp(name, "Handling") == 0)) // CHRISL HAM 4:


>>>>>>>>>>>>>>line 691

		else if(strcmp(name, "ubRecoilDelay") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.ubRecoilDelay = (INT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "Handling") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.ubHandling = (UINT8) atol(pData->szCharData);
		}
		//kenkenkenken: IoV921+z.5 -->
		else if(strcmp(name, "MalfunctionRate") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
            pData->curWeapon.MalfunctionRate = (UINT8) atol(pData->szCharData);
		}
		//<-- IoV

		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}

>>>>>>>>>>>>>>line 871

			FilePrintf(hFile,"\t\t<bBurstAP>%d</bBurstAP>\r\n",				Weapon[cnt].bBurstAP);
			FilePrintf(hFile,"\t\t<bAutofireShotsPerFiveAP>%d</bAutofireShotsPerFiveAP>\r\n",	Weapon[cnt].bAutofireShotsPerFiveAP);
			FilePrintf(hFile,"\t\t<APsToReload>%d</APsToReload>\r\n",							Weapon[cnt].APsToReload);
			FilePrintf(hFile,"\t\t<SwapClips>%d</SwapClips>\r\n",								Weapon[cnt].swapClips );
			FilePrintf(hFile,"\t\t<MaxDistForMessyDeath>%d</MaxDistForMessyDeath>\r\n",			Weapon[cnt].maxdistformessydeath);
			FilePrintf(hFile,"\t\t<AutoPenalty>%d</AutoPenalty>\r\n",			Weapon[cnt].AutoPenalty);
			FilePrintf(hFile,"\t\t<NoSemiAuto>%d</NoSemiAuto>\r\n",			Weapon[cnt].NoSemiAuto);
			FilePrintf(hFile,"\t\t<ubAimLevels>%d</ubAimLevels>\r\n",							Weapon[cnt].ubAimLevels );
			FilePrintf(hFile,"\t\t<EasyUnjam>%d</EasyUnjam>\r\n",			Weapon[cnt].EasyUnjam);
			FilePrintf(hFile,"\t\t<Handling>%d</Handling>\r\n",			Weapon[cnt].ubHandling);
			FilePrintf(hFile,"\t\t<MalfunctionRate>%d</MalfunctionRate>\r\n",			Weapon[cnt].MalfunctionRate); //kenkenkenken: IoV921+z.5


>>>>>>>>>>>>>>line 1133

��ԭ�����ʺ��� CheckForGunJam ����
���� IoV �µĹ����ʺ���

//kenkenkenken: IoV921+z.5 Add CheckForGunJam Function --> IoV�Զ�������ʺ���
BOOLEAN CheckForGunJamIoV( SOLDIERTYPE * pSoldier ) 
{ 
	OBJECTTYPE * pObj; 
 
	if ( ( pSoldier->flags.uiStatusFlags & SOLDIER_PC ) || ( pSoldier->flags.uiStatusFlags & SOLDIER_ENEMY ) ) 
	{		 
		if ( Item[pSoldier->usAttackingWeapon].usItemClass == IC_GUN && !EXPLOSIVE_GUN( pSoldier->usAttackingWeapon ) ) 
		{ 
			pObj = &(pSoldier->inv[pSoldier->ubAttackingHand]); 
			if ((*pObj)[0]->data.gun.bGunAmmoStatus > 0) 
			{ 
				int malfunctionRate = Weapon[pObj->usItem].MalfunctionRate;

				if (malfunctionRate == 0)
					malfunctionRate = 255;

				int condition = (*pObj)[0]->data.gun.bGunStatus;

				int malfunctionRateDivisor;

				if ( !pSoldier->bDoBurst )
				{
					malfunctionRateDivisor = gGameExternalOptions.iMalfunctionRateDivisorBasic;
				}
				else
				{
					malfunctionRateDivisor = gGameExternalOptions.iMalfunctionRateDivisorBurst;
				}

				if (condition == 100) //zwwooooo IoV924 z.6b1: If condition is 100, then reduce half jamChance
				{
					malfunctionRateDivisor = ( malfunctionRateDivisor * 50 ) / 100;
				}
				
				int jamChance = ( ( malfunctionRate * condition ) / malfunctionRateDivisor ) - gGameExternalOptions.ubWeaponReliabilityReductionPerRainIntensity * gbCurrentRainIntensity;

				if (jamChance < 0) 
					jamChance = 0;
				
				if ( !PreRandom( jamChance ) || gfNextFireJam )				
				{ 
					gfNextFireJam = FALSE; 
				 
					(*pObj)[0]->data.gun.bGunAmmoStatus *= -1; 
				 
					DeductAmmo( pSoldier, pSoldier->ubAttackingHand ); 
				 
					if (pSoldier->flags.uiStatusFlags & SOLDIER_PC){
						TacticalCharacterDialogue( pSoldier, QUOTE_JAMMED_GUN );
					}else{
						ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, sEnemyTauntsGunJam[ 7 ], pSoldier->name );				 
						StartEnemyTaunt( pSoldier, TAUNT_GUN_JAM );
					}					
					return( TRUE ); 
				} 
			} 
			else if ((*pObj)[0]->data.gun.bGunAmmoStatus < 0) 
			{ 
				if(EnoughPoints(pSoldier, APBPConstants[AP_UNJAM], APBPConstants[BP_UNJAM], FALSE))
				{
					DeductPoints(pSoldier, APBPConstants[AP_UNJAM], APBPConstants[BP_UNJAM]);
					INT8 bChanceMod;

					int iResult;
					
					if (pSoldier->flags.uiStatusFlags & SOLDIER_PC){

						if ( Weapon[pSoldier->inv[pSoldier->ubAttackingHand].usItem].EasyUnjam )
							bChanceMod = 100;
						else
							bChanceMod = (INT8) ((Item[pObj->usItem].bReliability + Item[(*pObj)[0]->data.gun.usGunAmmoItem].bReliability)* 4);

						iResult = SkillCheck( pSoldier, UNJAM_GUN_CHECK, bChanceMod);

					}else{						
						
						if (!PreRandom( 2 )){
							iResult = 0;
						}else{
							iResult = 1;
						}

					}
					
					if (iResult > 0) 
					{ 
						(*pObj)[0]->data.gun.bGunAmmoStatus *= -1; 
					 
						if (pSoldier->flags.uiStatusFlags & SOLDIER_PC){
							if (bChanceMod < 100)
							{
								StatChange( pSoldier, MECHANAMT, 5, FALSE ); 
								StatChange( pSoldier, DEXTAMT, 5, FALSE ); 
							}					 
							DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 ); 
						}else{
							ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, sEnemyTauntsGunJam[ 8 ], pSoldier->name );					 
							StartEnemyTaunt( pSoldier, TAUNT_GUN_UNJAM_S );
						}
					 
						return( 255 ); 
					} 
					else 
					{ 
						if (pSoldier->flags.uiStatusFlags & SOLDIER_ENEMY){
							ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, sEnemyTauntsGunJam[ 9 ], pSoldier->name );						
							StartEnemyTaunt( pSoldier, TAUNT_GUN_UNJAM_F );
						}
						return( TRUE ); 
					} 
				}
				else
					return( TRUE );
			} 
		} 		
	}
	return( FALSE ); 
}
//<--IoV end.

>>>>>>>>>>>>>>line 1396

\\\\\\\\�޸ĵ��ú���

	//bGunJamVal = CheckForGunJam( pSoldier );
	bGunJamVal = CheckForGunJamIoV( pSoldier ); //kenkenkenken: IoV921+z.5 ����IoV�Զ�������ʺ���


------------------------------------------------------------------------------------
Tactical\Weapons.h
------------------------------------------------------------------------------------

>>>>>>>>>>>>>>line 299

 INT16	sAniDelay;			// Lesh: for burst animation delay
 UINT8	APsToReloadManually;
 UINT16 ManualReloadSound;
 BOOLEAN EasyUnjam; // Guns where each bullet has its own chamber (like revolvers) are easyer to unjam 
 UINT8 MalfunctionRate; //kenkenkenken: IoV921+z.5

 INT8	bRecoilX;		// HEADROCK HAM 4: Recoil now measured in points of muzzle deviation X and Y.
 INT8	bRecoilY;		// Positive values indicated upwards (Y) and rightwards (X). Negatives are down (-Y) and left (-X).

>>>>>>>>>>>>>>line 389

extern BOOLEAN	OKFireWeapon( SOLDIERTYPE *pSoldier );
extern BOOLEAN CheckForGunJam( SOLDIERTYPE * pSoldier );

extern BOOLEAN CheckForGunJamIoV( SOLDIERTYPE * pSoldier ); //kenkenkenken: IoV921+z.5


------------------------------------------------------------------------------------
Tactical\Soldier Ani.cpp
------------------------------------------------------------------------------------

>>>>>>>>>>>>>>line 775

					//bWeaponJammed = CheckForGunJam( pSoldier );
					bWeaponJammed = CheckForGunJamIoV( pSoldier ); //kenkenkenken: IoV921+z.5


------------------------------------------------------------------------------------
\Tactical\Civ Quotes.h
------------------------------------------------------------------------------------

>>>>>>>>>>>>>>line 63

enum //enemy taunts - SANDRO
{
	//kenkenkenken: IoV921+z.5 -->
	TAUNT_GUN_JAM, 
	TAUNT_GUN_UNJAM_S,
	TAUNT_GUN_UNJAM_F, 
	//<-- IoV
	TAUNT_FIRE_GUN,
	TAUNT_FIRE_LAUNCHER,
	TAUNT_THROW,
	TAUNT_CHARGE_KNIFE,
	TAUNT_RUN_AWAY,
	TAUNT_SEEK_NOISE,
	TAUNT_ALERT,
	TAUNT_GOT_HIT
};


------------------------------------------------------------------------------------
\Tactical\Civ Quotes.cpp
------------------------------------------------------------------------------------

>>>>>>>>>>>>>>line 994

		case TAUNT_GOT_HIT:
			iTauntNumber = Random(7);
			sTauntText = sEnemyTauntsGotHit[iTauntNumber];
			break;
		//kenkenkenken: IoV921+z.5 -->
		case TAUNT_GUN_JAM:
			iTauntNumber = Random(3);
			sTauntText = sEnemyTauntsGunJam[iTauntNumber];
			break;
		case TAUNT_GUN_UNJAM_S:
			iTauntNumber = Random(2);
			sTauntText = sEnemyTauntsGunJam[iTauntNumber + 3];
			break;
		case TAUNT_GUN_UNJAM_F:
			iTauntNumber = Random(2);
			sTauntText = sEnemyTauntsGunJam[iTauntNumber + 5];
			break;
		//<-- IoV
		default:
			return;
			break;

------------------------------------------------------------------------------------
\Utils\Text.h
------------------------------------------------------------------------------------

>>>>>>>>>>>>>>line 257
			
		extern STR16 sEnemyTauntsAlert[];
extern STR16 sEnemyTauntsGotHit[];
//****

extern STR16 sEnemyTauntsGunJam[]; //kenkenkenken: IoV921+z.5

// HEADROCK HAM 3.6: New arrays for facility operation messages
extern STR16 gzFacilityErrorMessage[];
extern STR16 gzFacilityAssignmentStrings[];
extern STR16 gzFacilityRiskResultStrings[];

	
------------------------------------------------------------------------------------
\Utils\_ChineseText.cpp
------------------------------------------------------------------------------------

>>>>>>>>>>>>>>line 6346

STR16 Additional113Text[]=
{
	L"Jagged Alliance 2 v1.13 ����ģʽ��Ҫһ��16bpp����ٵ���ɫ��ȡ�",
	
	// WANNE: Savegame slots validation against INI file
	L"Internal error in reading %s slots from Savegame: Number of slots in Savegame (%d) differs from defined slots in ja2_options.ini settings (%d)",
	L"Mercenary (MAX_NUMBER_PLAYER_MERCS) / Vehicle (MAX_NUMBER_PLAYER_VEHICLES)", 
	L"Enemy (MAX_NUMBER_ENEMIES_IN_TACTICAL)", 
	L"Creature (MAX_NUMBER_CREATURES_IN_TACTICAL)", 
	L"Militia (MAX_NUMBER_MILITIA_IN_TACTICAL)", 
	L"Civilian (MAX_NUMBER_CIVS_IN_TACTICAL)",

};

//kenkenkenken: IoV921+z.5 -->
STR16 sEnemyTauntsGunJam[]=
{
	L"������! �ؼ�ʱ�̿���!",
	L"��ţ����! ǹ��ס��!",
	L"��ô����! ���ո��ڴ�!",

	L"�Ŵ��粻����! ŶҲ!",
	L"�ս������������!",

	L"���߰�! ���ǿ�!",
	L"�Ҳ�! Ū������!",

	L"�б�������������!",
	L"�б��ų����ǳɹ�!",
	L"�б��ų�����ʧ��!",
};
//<-- IoV

// SANDRO - Taunts (here for now, xml for future, I hope)
// MINTY - Changed some of the following taunts to sound more natural

>>>>>>>>>>>>>>line 2245

CHAR16		gWeaponStatsDesc[][ 28 ] = //kenkenkenken: IoV921+z.5 Altered value from 17 to 28
{
	// HEADROCK: Changed this for Extended Description project
	L"״̬: ",
	L"����: ",
	L"AP ����",
	L"���: ",		// Range
	L"ɱ����: ",		// Damage
	L"��ҩ", 	// Number of bullets left in a magazine
	L"AP: ",			// abbreviation for Action Points
	L"=",
	L"=",
					//Lal: additional strings for tooltips
	L"׼ȷ��: ", //9
	L"���: ", //10
	L"ɱ����: ", //11
	L"����: ", //12
	L"��ѣɱ����: ",//13
	// HEADROCK: Added new strings for extended description ** REDUNDANT **
	// HEADROCK HAM 3: Replaced #14 with string for Possible Attachments (BR's Tooltips Only)
	// Obviously, THIS SHOULD BE DONE IN ALL LANGUAGES...
	L"����:",	//14 //ham3.6
	L"����/5AP: ",		//15
	L"ʣ�൯ҩ:",		//16
	L"Ĭ��:",	//17 //WarmSteel - So we can also display default attachments
	//kenkenkenken: IoV921+z.5 -->
	L"������:",		        //18
	L"x----",           //19
	L"o----",          //20
	L"ox---",         //21
	L"oo---",        //22
	L"oox--",       //23
	L"ooo--",      //24
	L"ooox-",     //25
	L"oooo-",    //26
	L"oooox",   //27
	L"ooooo",  //28
	//<-- IoV

};

>>>>>>>>>>>>>>line 6548

STR16 szUDBGenWeaponsStatsTooltipText[]=
{
	L"|��|��",
	L"|ɱ|��|��",
	L"|��|��",
	L"|��|��|��|��",
	L"|��|׼|��|��|��",
	L"|��|��|Ч|��",
	L"|��|��",
	L"|��|��",
	L"|��|��|��",
	L"|��|��|��|��",
	//L"|��|��|��|��|��|Ч|��|��",
	L"|��|��|��", //zwwooooo: IoV921+z.5
	L"|��|��|��|��|��",
	L"", // (12)
	L"|��|ǹ|A|P",
	L"|��|��|A|P",
	L"|��|��|A|P",
	L"|��|��|A|P",
	L"|��|��|A|P",
	L"|��|��|��|A|P",
	L"|ˮ|ƽ|��|��|��",
	L"|��|ֱ|��|��|��",
	L"|��|��|��|��|��/5|A|P",
};

STR16 szUDBGenWeaponsStatsExplanationsTooltipText[]=
{
	L"\n \n����������������ӵ�ƫ����׼���Զ����\n \n��ֵ��Χ��0~100��Խ��Խ�á�",
	L"\n \n����������������ӵ���ƽ���˺���������\n���׻򴩼�������\n \n����ֵԽ��Խ�á�",
	L"\n \n�Ӹ�ǹ������ӵ������ǰ��\n�����о��루��������\n \n����ֵԽ��Խ�á�",
	L"\n \n�����ֵ��ʾ�˸������ľ���ȼ���\n \n����ȼ�Խ�ͣ�ÿ����׼��õ������ʼӳ�Խ\n�ߡ���ˣ�����ȼ�Ҫ��ϵ͵����������ڲ���\nʧ���ȵ�����¸������׼��\n \n����ֵԽ��Խ�á�",
	L"\n \n��ֵ����1.0ʱ��Զ������׼�е����ᰴ������С��\n \n \n��ע�⣬�߱�����׼��������������������Ŀ�ꡣ\n \n \n��ֵΪ1.0��˵��������δ��װ��׼����\n",
	L"\n \n��һ�������ϰ�����������׼��\n \n���Ч��ֻ��һ����������Ч����Ϊ�����þ���\n���ͻῪʼ�������������㹻Զ����ʧ��\n \n����ֵԽ��Խ�á�",
	L"\n \n������������ʱ���������䲻�����ǹ�档\n \n \n���˲���ͨ��ǹ���ж����λ�ã�����������Ȼ��\n�������㡣",
	L"\n \n�ò�����ʾ����������ʱǹ��������\n��Χ����������\n \n�˷�Χ�ڵĵ��˾��п�������ǹ����\n \n����ֵԽ��Խ�á�",
	L"\n \n�����˸�����ʹ��ʱ��ĵĿ�����\n \n����ֵԽ��Խ�á�",
	L"\n \n�����������������Ѷȡ�\n����ֵԽ��Խ�á�",
	//L"\n \n��׼���ṩ��׼�����ʼӳɵ���̾��롣\n���ٽ�����Ч�ˣ�",
	L"\n \n�����������������ϵļ��ʡ�\n \n�ٵ�XXOO�ȽϺá�", //zwwooooo: IoV921+z.5
	L"\n \n������׼���ṩ��������������",
	L"", // (12)
	L"\n \n��ǹ׼�����������AP��\n \n������������������䲻��������\n��ǹAP��\n \n���ǣ���ת��Ϳ���֮����������������\n��������\n \n����ֵԽ��Խ�á�",
	L"\n \n���������������ҩ�����AP��\n����ǹе���ԣ�\n����ֵ��ʾ���ڲ����������·���\nһ���ӵ���AP���ġ�\n \n�����ͼ��Ϊ��ɫ������������ɵ��������\n \n����ֵԽ��Խ�á�",
	L"\n \nһ�ε��������AP��\n \nÿ�ε�����ӵ�����ǹ֧���������\n����ʾ�ڸ�ͼ���ϡ�\n \n�����ͼ�귢�ң�����������ɵ��䡣\n \n����ֵԽ��Խ�á�",
	L"\n \n����ģʽ�£�������һ��\n���������ӵ������AP��\n \n����3���ӵ���\n����Ҫ�����AP��\n \n�����ͼ�귢�ң������������������\n \n����ֵԽ��Խ�á�",
	L"\n \n����װ���ӵ������AP��\n \n����ֵԽ��Խ�á�",
	L"\n \n�������ЪΪ�������ֶ�������ϻ��AP���ġ�\n \n����ֵԽ��Խ�á�",
	L"\n \n�ò�����ʾ��ǹ���ڵ��������ģʽ�·����ӵ�\nʱ��ˮƽλ�ơ�\n \n���ò���Ϊ��ʱ��ǹ����ƫ����֮��������ƫ��\n \nԽ�ӽ�0Խ�á�",
	L"\n \n�ò�����ʾ��ǹ���ڵ��������ģʽ�·����ӵ�\nʱ�Ĵ�ֱλ�ơ�\n \n���ò���Ϊ��ʱ��ǹ����ƫ����֮��������ƫ��\n \nԽ�ӽ�0Խ�á�",
	L"\n \n�ò�����ʾ�˸�����ÿ�໨��5AP������ģʽʱ\n�ɶ෢����ӵ�����\n \n����ֵԽ��Խ�á�",
};

------------------------------------------------------------------------------------
\Utils\_EnglishText.cpp
------------------------------------------------------------------------------------

>>>>>>>>>>>>>>line 6341

STR16 Additional113Text[]=
{
	L"Jagged Alliance 2 v1.13 windowed mode requires a color depth of 16bpp or less.",
	
	// WANNE: Savegame slots validation against INI file
	L"Internal error in reading %s slots from Savegame: Number of slots in Savegame (%d) differs from defined slots in ja2_options.ini settings (%d)",
	L"Mercenary (MAX_NUMBER_PLAYER_MERCS) / Vehicle (MAX_NUMBER_PLAYER_VEHICLES)", 
	L"Enemy (MAX_NUMBER_ENEMIES_IN_TACTICAL)", 
	L"Creature (MAX_NUMBER_CREATURES_IN_TACTICAL)", 
	L"Militia (MAX_NUMBER_MILITIA_IN_TACTICAL)", 
	L"Civilian (MAX_NUMBER_CIVS_IN_TACTICAL)",

};

//kenkenkenken: IoV921+z.5 -->
STR16 sEnemyTauntsGunJam[]=
{
	L"What the fuck??? Jammed!",
	L"Aww FUUUUCK! Gun jammed!",
	L"Gun jammed! Arrrrrrrrgh!",

	L"Yay! Unjammed finally!",
	L"We're good! Unjammed!!",

	L"No! Come the fuck on!!!",
	L"God help me unjam this!",

	L"Enemy's gun jammed!",
	L"Enemy successfully unjammed!",
	L"Enemy failed to unjam the gun!",
};
//<-- IoV

// SANDRO - Taunts (here for now, xml for future, I hope)
// MINTY - Changed some of the following taunts to sound more natural

>>>>>>>>>>>>>>line 2246

CHAR16		gWeaponStatsDesc[][ 28 ] = //kenkenkenken: IoV921+z.5 Altered value from 17 to 28
{
	// HEADROCK: Changed this for Extended Description project
	L"Status:",
	L"Weight:", 
	L"AP Costs",	
	L"Rng:",		// Range
	L"Dam:",		// Damage
	L"Amount:", 	// Number of bullets left in a magazine
	L"AP:",			// abbreviation for Action Points
	L"=",
	L"=",
					//Lal: additional strings for tooltips
	L"Accuracy:",	//9
	L"Range:",		//10	
	L"Damage:", 	//11
	L"Weight:",		//12
	L"Stun Damage:",//13
	// HEADROCK: Added new strings for extended description ** REDUNDANT **
	// HEADROCK HAM 3: Replaced #14 with string for Possible Attachments (BR's Tooltips Only)
	// Obviously, THIS SHOULD BE DONE IN ALL LANGUAGES...
	L"Attachments:",	//14
	L"AUTO/5:",		//15
	L"Remaining ammo:",		//16
	L"Default:",	//17 //WarmSteel - So we can also display default attachments
	//kenkenkenken: IoV921+z.5 -->
    L"MALF:",		      //18 
	L"x----",           //19
	L"o----",          //20
	L"ox---",         //21
	L"oo---",        //22
	L"oox--",       //23
	L"ooo--",      //24
	L"ooox-",     //25
	L"oooo-",    //26
	L"oooox",   //27
	L"ooooo",  //28
	//<-- IoV

};

>>>>>>>>>>>>>>line 6529

STR16 szUDBGenWeaponsStatsTooltipText[]=
{
	L"|A|c|c|u|r|a|c|y",
	L"|D|a|m|a|g|e",
	L"|R|a|n|g|e",
	L"|A|l|l|o|w|e|d |A|i|m|i|n|g |L|e|v|e|l|s",
	L"|S|c|o|p|e |M|a|g|n|i|f|i|c|a|t|i|o|n |F|a|c|t|o|r",
	L"|P|r|o|j|e|c|t|i|o|n |F|a|c|t|o|r",
	L"|H|i|d|d|e|n |M|u|z|z|l|e |F|l|a|s|h",
	L"|L|o|u|d|n|e|s|s",
	L"|R|e|l|i|a|b|i|l|i|t|y",
	L"|R|e|p|a|i|r |E|a|s|e",
	//L"|M|i|n|. |R|a|n|g|e |f|o|r |A|i|m|i|n|g |B|o|n|u|s",
	L"|M|a|l|f|u|n|c|t|i|o|n", //zwwooooo: IoV921+z.5
	L"|T|o|-|H|i|t |M|o|d|i|f|i|e|r",
	L"", // (12)
	L"|A|P|s |t|o |R|e|a|d|y",
	L"|A|P|s |t|o |A|t|t|a|c|k",
	L"|A|P|s |t|o |B|u|r|s|t",
	L"|A|P|s |t|o |A|u|t|o|f|i|r|e",
	L"|A|P|s |t|o |R|e|l|o|a|d",
	L"|A|P|s |t|o |R|e|c|h|a|m|b|e|r",
	L"|L|a|t|e|r|a|l |R|e|c|o|i|l",
	L"|V|e|r|t|i|c|a|l |R|e|c|o|i|l",
	L"|A|u|t|o|f|i|r|e |B|u|l|l|e|t|s |p|e|r |5 |A|P|s",
};

STR16 szUDBGenWeaponsStatsExplanationsTooltipText[]=
{
	L"\n \nDetermines whether bullets fired by\nthis gun will stray far from where\nit is pointed.\n \nScale: 0-100.\nHigher is better.",
	L"\n \nDetermines the average amount of damage done\nby bullets fired from this weapon, before\ntaking into account armor or armor-penetration.\n \nHigher is better.",
	L"\n \nThe maximum distance (in tiles) that\nbullets fired from this gun will travel\nbefore they begin dropping towards the\nground.\n \nHigher is better.",
	L"\n \nThis is the number of Extra Aiming\nLevels you can add when aiming this gun.\n \nThe FEWER aiming levels are allowed, the MORE\nbonus each aiming level gives you. Therefore,\nhaving FEWER levels makes the gun faster to aim,\nwithout making it any less accurate.\n \nLower is better.",
	L"\n \nWhen greater than 1.0, will proportionally reduce\naiming errors at a distance.\n \nRemember that high scope magnification is detrimental\nwhen the target is too close!\n \nA value of 1.0 means no scope is installed.",
	L"\n \nProportionally reduces aiming errors at a distance.\n \nThis effect works up to a given distance,\nthen begins to dissipate and eventually\ndisappears at sufficient range.\n \nHigher is better.",
	L"\n \nWhen this property is in effect, the weapon\nproduces no visible flash when firing.\n \nEnemies will not be able to spot you\njust by your muzzle flash (but they\nmight still HEAR you).",
	L"\n \nWhen firing this weapon, Loudness is the\ndistance (in tiles) that the sound of\ngunfire will travel.\n \nEnemies within this distance will probably\nhear the shot.\n \nLower is better.",
	L"\n \nDetermines how quickly this weapon will degrade\nwith use.\n \nHigher is better.",
	L"\n \nDetermines how difficult it is to repair this weapon.\n \nHigher is better.",
	//L"\n \nThe minimum range at which a scope can provide it's aimBonus.",
	L"\n \n Weapons' malfunction rate.\n \nLess OOXX is better.", //zwwooooo: IoV921+z.5
	L"\n \nTo hit modifier granted by laser sights.",
	L"", // (12)
	L"\n \nThe number of APs required to bring this\nweapon up to firing stance.\n \nOnce the weapon is raised, you may fire repeatedly\nwithout paying this cost again.\n \nA weapon is automatically 'Unreadied' if its\nwielder performs any action other than\nfiring or turning.\n \nLower is better.",
	L"\n \nThe number of APs required to perform\na single attack with this weapon.\n \nFor guns, this is the cost of firing\na single shot without extra aiming.\n \nIf this icon is greyed-out, single-shots\n are not possible with this weapon.\n \nLower is better.",
	L"\n \nThe number of APs required to fire\na burst.\n \nThe number of bullets fired in each burst is\ndetermined by the weapon itself, and indicated\nby the number of bullets shown on this icon.\n \nIf this icon is greyed-out, burst fire\nis not possible with this weapon.\n \nLower is better.",
	L"\n \nThe number of APs required to fire\nan Autofire Volley of three bullets.\n \nIf you wish to fire more than 3 bullets,\nyou will need to pay extra APs.\n \nIf this icon is greyed-out, autofire\nis not possible with this weapon.\n \nLower is better.",
	L"\n \nThe number of APs required to reload\nthis weapon.\n \nLower is better.",
	L"\n \nThe number of APs required to rechamber this weapon\nbetween each and every shot fired.\n \nLower is better.",
	L"\n \nThe distance this weapon's muzzle will shift\nhorizontally between each and every bullet in a\nburst or autofire volley.\n \nPositive numbers indicate shifting to the right.\nNegative numbers indicate shifting to the left.\n \nCloser to 0 is better.",
	L"\n \nThe distance this weapon's muzzle will shift\nvertically between each and every bullet in a\nburst or autofire volley.\n \nPositive numbers indicate shifting upwards.\nNegative numbers indicate shifting downwards.\n \nCloser to 0 is better.",
	L"\n \nIndicates the number of bullets that will be added\nto an autofire volley for every extra 5 APs\nyou spend.\n \nHigher is better.",
};


------------------------------------------------------------------------------------
\Utils\Text.h
------------------------------------------------------------------------------------

>>>>>>>>>>>>>>line 540

extern CHAR16		gMoneyStatsDesc[][ 13 ];
// HEADROCK: Altered value to 16 //WarmSteel - And I need 17.
extern CHAR16		gWeaponStatsDesc[][ 28 ]; //kenkenkenken: IoV921+z.5 = 28, 1.13 = 17

// HEADROCK: Added externs for Item Description Box icon and stat tooltips


===================================================================================================================

IoV921+ : Я�߲��µ����� Դ�� kenkenkenken �޸����ϸ��� zwwoooooo

===================================================================================================================

------------------------------------------------------------------------------------
Tactical\Weapons.cpp
------------------------------------------------------------------------------------

>>>>>>>>>>>>>>line 974
\\\\\\\\��
INT32 ArmourPercent( SOLDIERTYPE * pSoldier )
\\\\\\\\ǰ�����kenkenkenkenд��IoVר�ò�庯��

//kenkenkenken: IoV921+z.5 ������ -->
INT32 EffectiveArmourLBE( OBJECTTYPE * pObj )
{
	INT32		iValue;

	if (pObj == NULL || Item[pObj->usItem].usItemClass != IC_LBEGEAR)
	{
		return( 0 );
	}
	
	iValue = 0;
	
	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		if (Item[iter->usItem].usItemClass == IC_ARMOUR && (*iter)[0]->data.objectStatus > 0 )
		{
			INT32 iValue2;

			iValue2 = Armour[ Item[ iter->usItem ].ubClassIndex ].ubProtection;
			iValue2 = iValue2 * (*iter)[0]->data.objectStatus * Armour[ Item[ iter->usItem ].ubClassIndex ].ubCoverage / 10000;

			iValue += iValue2;
		}
	}
	return( max(iValue,0) );
}

INT32 ExplosiveEffectiveArmourLBE( OBJECTTYPE * pObj )
{
	INT32		iValue;

	if (pObj == NULL || Item[pObj->usItem].usItemClass != IC_LBEGEAR)
	{
		return( 0 );
	}
	
	iValue = 0;
	
	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		if (Item[iter->usItem].usItemClass == IC_ARMOUR && (*iter)[0]->data.objectStatus > 0 )
		{
			INT32 iValue2;

			iValue2 = Armour[ Item[ iter->usItem ].ubClassIndex ].ubProtection;
			iValue2 *= (*iter)[0]->data.objectStatus * Armour[ Item[ iter->usItem ].ubClassIndex ].ubCoverage / 10000;

			iValue += iValue2;
		}
	}
	return( max(iValue,0) );
}
//<-- IoV

>>>>>>>>>>>>>>line ����

\\\\\\\�޸�INT32 ArmourPercent( SOLDIERTYPE * pSoldier )��������벿��

	//kenkenkenken: IoV921+z.5 ���Ӳ�����Ч�� -->
	INT32 iVestPack;
	if (pSoldier->inv[VESTPOCKPOS].exists() == true)
	{
		iVestPack = EffectiveArmourLBE( &(pSoldier->inv[VESTPOCKPOS]) );
		iDivideValue = ( ( Armour[ Item[ SPECTRA_VEST_18 ].ubClassIndex ].ubProtection * Armour[ Item[ SPECTRA_VEST_18 ].ubClassIndex ].ubCoverage ) + ( Armour[ Item[ CERAMIC_PLATES ].ubClassIndex ].ubProtection * Armour[ Item[ CERAMIC_PLATES ].ubClassIndex ].ubCoverage ) );

		// WANNE: Just to be on the save side
		if (iDivideValue > 0)
		{
			// convert to % of best; ignoring bug-treated stuff
			iVestPack = 6500 * iVestPack / iDivideValue;
		}
		else
		{
			iVestPack = 65 * iVestPack / ( Armour[ Item[ SPECTRA_VEST_18 ].ubClassIndex ].ubProtection + Armour[ Item[ CERAMIC_PLATES ].ubClassIndex ].ubProtection );
		}
	}
	else
	{
		iVestPack = 0;
	}
	return( (iHelmet + iVest + iLeg + iVestPack) );
	//<-- IoV
	//return( (iHelmet + iVest + iLeg) );
}

>>>>>>>>>>>>>>line ����

\\\\\\\�޸�INT32 INT8 ArmourVersusExplosivesPercent( SOLDIERTYPE * pSoldier )��������벿��

	//kenkenkenken: IoV921+z.5 ������Ч�� -->
	INT32 iVestPack;
	if (pSoldier->inv[VESTPOCKPOS].exists() == true)
	{
		iVestPack = ExplosiveEffectiveArmourLBE( &(pSoldier->inv[VESTPOCKPOS]) );
		// convert to % of best; ignoring bug-treated stuff
		iVestPack = __min( 65, 6500 * iVestPack / ( ( Armour[ Item[ SPECTRA_VEST_18 ].ubClassIndex ].ubProtection * Armour[ Item[ SPECTRA_VEST_18 ].ubClassIndex ].ubCoverage ) + ( Armour[ Item[ CERAMIC_PLATES ].ubClassIndex ].ubProtection * Armour[ Item[ CERAMIC_PLATES ].ubClassIndex ].ubCoverage ) ) );
	}
	else
	{
		iVestPack = 0;
	}
	return( (INT8) (iHelmet + iVest + iLeg + iVestPack) );
	//<-- IoV
	//return( (INT8) (iHelmet + iVest + iLeg) );
}

>>>>>>>>>>>>>>line 8445

\\\\\\\�ҵ�
pArmour = &(pTarget->inv[ iSlot ]);
\\\\\\\\����ǰ�����IoV��ע����

		//kenkenkenken: IoV921+z.5 ������ -->
		OBJECTTYPE *	pVestPack;
		pVestPack = &(pTarget->inv[VESTPOCKPOS]);
		if (iSlot == VESTPOS && pVestPack->exists() == true)
		{
			for (attachmentList::iterator iter = (*pVestPack)[0]->attachments.begin(); iter != (*pVestPack)[0]->attachments.end(); ++iter) {
				if (Item[iter->usItem].usItemClass == IC_ARMOUR && (*iter)[0]->data.objectStatus > 0 )
				{
					iTotalProtection += ArmourProtection( pTarget, Item[iter->usItem].ubClassIndex, &((*iter)[0]->data.objectStatus), iImpact, ubAmmoType, &plateHit );
					if ( (*iter)[0]->data.objectStatus < USABLE )
					{
						pVestPack->RemoveAttachment(&(*iter));
						DirtyMercPanelInterface( pTarget, DIRTYLEVEL2 );
						if ( pTarget->bTeam == gbPlayerNum )
						{
							ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, gzLateLocalizedString[61], pTarget->name );
						}
					}
					break;
				}
			}
		}
				
		if ( iImpact > iTotalProtection )
		{
		//<--IoV
		pArmour = &(pTarget->inv[ iSlot ]);
		if (pArmour->exists() == true)
		{
			for (attachmentList::iterator iter = (*pArmour)[0]->attachments.begin(); iter != (*pArmour)[0]->attachments.end(); ++iter) {
				if (Item[iter->usItem].usItemClass == IC_ARMOUR && (*iter)[0]->data.objectStatus > 0 && iter->exists())
				{
					// bullet got through jacket; apply ceramic plate armour
������������������
������������������
\\\\\\\\��������ע��������� IoV ��ע���֣��Ҵ����ţ�
		}
		} //kenkenkenken: IoV921+z.5 Add
	}
	return( iTotalProtection );
}


===================================================================================================================

IoV921+ z.6b2 Test: �Զ��� BR ÿ���������� | by zwwoooooo
ע�������ã�δ����IoV

===================================================================================================================

------------------------------------------------------------------------------------
\Laptop\LaptopSave.h
------------------------------------------------------------------------------------
#define		MAX_PURCHASE_AMOUNT								10   //����BRÿ�����������
\\\\\\\\��Ϊ
#define		MAX_PURCHASE_AMOUNT								gGameExternalOptions.iMaxPurchaseAmountIoV


------------------------------------------------------------------------------------
\Laptop\BobbyRGuns.cpp
------------------------------------------------------------------------------------
>>>>>>>>>>>>>>line 1021 BR��Ʒ�����嵥ֻ����ʾ10����Ʒ������Ҫ������ʾǰ��10��
	for(i=0; i<MAX_PURCHASE_AMOUNT; i++)
\\\\\\\\��Ϊ
	//for(i=0; i<MAX_PURCHASE_AMOUNT; i++)
	//zwwooooo: IoV921+ z.6b2 : Custom MAX_PURCHASE_AMOUNT -->
	int MAX_PURCHASE_AMOUNT_IoV = MAX_PURCHASE_AMOUNT;
	if (MAX_PURCHASE_AMOUNT_IoV>10) MAX_PURCHASE_AMOUNT_IoV = 10; //limit MAX_PURCHASE_AMOUNT_IoV <= 10, because BR only display 10 purchased items
	for(i=0; i<MAX_PURCHASE_AMOUNT_IoV; i++)
	//IoV921+ z.6b2 <--

------------------------------------------------------------------------------------
\Laptop\BobbyRGuns.cpp
------------------------------------------------------------------------------------
BobbyRText[ BOBBYR_MORE_THEN_10_PURCHASES ] ������������10��ľ����־�

>>>>>>>>>>>>>>line 2974 �� 3020
DoLapTopMessageBox( MSG_BOX_LAPTOP_DEFAULT, BobbyRText[ BOBBYR_MORE_THEN_10_PURCHASES ], LAPTOP_SCREEN, MSG_BOX_FLAG_OK, NULL);
\\\\\\\\��Ϊ
					//DoLapTopMessageBox( MSG_BOX_LAPTOP_DEFAULT, BobbyRText[ BOBBYR_MORE_THEN_10_PURCHASES ], LAPTOP_SCREEN, MSG_BOX_FLAG_OK, NULL);
					//zwwooooo: IoV921+ z.6b2 --> When Custom MAX_PURCHASE_AMOUNT message is change.
					if (MAX_PURCHASE_AMOUNT>10)
					{
					#ifdef CHINESE
						DoLapTopMessageBox( MSG_BOX_LAPTOP_DEFAULT, L"��!  ������������߶���һ��ֻ���� 60 ����Ʒ�Ķ������������Ҫ�������ණ��������ϣ����ˣ�����������ǵ�Ǹ�⣬�ٿ�һ�ݶ�����", LAPTOP_SCREEN, MSG_BOX_FLAG_OK, NULL);
					#else
						DoLapTopMessageBox( MSG_BOX_LAPTOP_DEFAULT, L"Darn!  This on-line order form will only accept 60 items per order.  If you're looking to order more stuff (and we hope you are), kindly make a separate order and accept our apologies.", LAPTOP_SCREEN, MSG_BOX_FLAG_OK, NULL);
					#endif
					}
					else
					{
						DoLapTopMessageBox( MSG_BOX_LAPTOP_DEFAULT, BobbyRText[ BOBBYR_MORE_THEN_10_PURCHASES ], LAPTOP_SCREEN, MSG_BOX_FLAG_OK, NULL);
					}
					//IoV921+ z.6b2 <--



------------------------------------------------------------------------------------

------------------------------------------------------------------------------------

>>>>>>>>>>>>>>line 



