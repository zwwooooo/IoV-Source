/*************************************************************
IoV源码修改方法记录        by zwwooooo - 2011.10.28

////////////////////////////////////
实现的功能

主谋：daboboye=叫兽

* 把<RangeBonus>从原来的实数增加改为比例增加 by kenkenkenken
* 白天和夜晚的视距从2/1变成3/1 by zwwooooo
* 就是这个 ItemSiz 的放大了。 by kenkenkenken, edit zwwooooo
* 解决了原程序如下问题：by kenkenkenken, edit zwwooooo
	1. 榴弹切换不消耗AP
	2. 枪挂榴弹装弹 按装附件算AP 而不是 按装弹算AP
	3. 加一个带弹的榴弹发射器到步枪上，会按装榴弹为附件算AP 而不是按装发射器为附件算AP ； 如果装一个没弹的榴弹发射器到步枪上，AP是对的
	4. 从步枪上拆卸一个带弹的榴弹发射器，AP计算的bug与第3点类似，但不是一定会遇到，关键是卸载是附件格子里是 弹在前面 还是 发射器在前面，如果是弹在前面会遇到此bug
////////////////////////////////////


=========== 代表实现的功能说明
----------- 代表要修改的文件名
>>>>>>>>>>> 要修改的大概行号（版本的升级会不同，搜索即可）
\\\\\\\\\\\ 说明注释


= 此说明文件记录最近更新时 ja2.exe 源码版本 =
-svn4769 2011.10.28
-


**************************************************************/


===================================================================================================================

是把<RangeBonus>从原来的实数增加改为比例增加

===================================================================================================================


------------------------------------------------------------------------------------------
Tactical\Weapons.cpp
------------------------------------------------------------------------------------------

>>>>>>>>>>>>>>大概 line914 

		UINT16 usRange = GetModifiedGunRange(pObj->usItem);

		// Snap: attachment status is factored into the range bonus calculation
		rng = usRange + GetRangeBonus(pObj);

\\\\\\查找：
rng = usRange + GetRangeBonus(pObj);
\\\\\\改为：
rng = ( Weapon[ pObj->usItem ].usRange * GetRangeBonusIoV(pObj) ) / 10000; //IoV 921 - r01


------------------------------------------------------------------------------------------
Tactical\Items.cpp
------------------------------------------------------------------------------------------

>>>>>>>>>>>>>>大概 line 9060 

\\\\\\找到下面这段

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

\\\\\\在其下面加入 IoV 专用的<RangeBonus>计算公式：比例增加

//DBB's IoV R919-003 --> by kenkenkenken
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
//<--DBB's IoV


>>>>>>>>>>>>>>大概 line（11373-19） 

	// Read from item
	aimLevels = Weapon[pSoldier->inv[pSoldier->ubAttackingHand].usItem].ubAimLevels;
	fTwoHanded = Item[pSoldier->inv[pSoldier->ubAttackingHand].usItem].twohanded;
	weaponRange = Weapon[pSoldier->inv[pSoldier->ubAttackingHand].usItem].usRange + GetRangeBonus(&pSoldier->inv[pSoldier->ubAttackingHand]);
	weaponType = Weapon[pSoldier->inv[pSoldier->ubAttackingHand].usItem].ubWeaponType;
	fUsingBipod = FALSE;

\\\\\\把里面的
weaponRange = Weapon[pSoldier->inv[pSoldier->ubAttackingHand].usItem].usRange + GetRangeBonus(&pSoldier->inv[pSoldier->ubAttackingHand]);
\\\\\\改为
weaponRange = ( Weapon[pSoldier->inv[pSoldier->ubAttackingHand].usItem].usRange * GetRangeBonusIoV(&pSoldier->inv[pSoldier->ubAttackingHand]) ) / 10000; //DBB's IoV R919-003 by kenkenkenken


------------------------------------------------------------------------------------------
Tactical\Items.h
------------------------------------------------------------------------------------------

>>>>>>>>>>>>>>line 249
\\\\\\在
INT16 GetRangeBonus( OBJECTTYPE * pObj );
\\\\\\下面增加
INT32 GetRangeBonusIoV( OBJECTTYPE * pObj ); //DBB's IoV R919-003 by kenkenkenken



===================================================================================================================

ItemSize 更改最大值为 54

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

\\\\\\里面的34改为54

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

\\\\\\34改为54

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

\\\\\\34改为54


---------------------------------------------
IoV 物品数组
解释：原版里，0～9是武器，10是超小物品，从11开始到34，每4个数字代表一个规格，不同数字代表不同形状，共6×4。

修改为：0～12是武器，12是超小物品，从13～54，每7个数字代表一个规格，不同数字代表不同形状，共6×7。 （by zwwooooo）
---------------------------------------------

>>>>>>>>>>>>>>line 2735(修改后)

	//for each object in the stack, hopefully there is only 1
	for (int numStacked = 0; numStacked < pObject->ubNumberOfObjects; ++numStacked) {
		//some weapon attachments can adjust the ItemSize of a weapon
		if(iSize<12) { // IoV 921-001:12 1.13:10 by zwwooooo
			for (attachmentList::iterator iter = (*pObject)[numStacked]->attachments.begin(); iter != (*pObject)[numStacked]->attachments.end(); ++iter) {
				if (iter->exists() == true) {
					iSize += Item[iter->usItem].itemsizebonus;
					// CHRISL: This is to catch things if we try and reduce ItemSize when we're already at 0
					if(iSize > 54 || iSize < 0) // IoV 921-001: 54 1.13:34
						iSize = 0;
					if(iSize > 11) // IoV 921-001:11 1.13:9
						iSize = 11; // IoV 921-001:11 1.13:9
				}
			}
		}

\\\\\\修改过的数值有 IoV 标识和说明


>>>>>>>>>>>>>>line 2683（修改后）

\\\\\\找到此函数

int GetPocketSizeByDimensions(int sizeX, int sizeY)
{
	static const UINT8 cisPocketSize[6][7] = // IoV 921-001:[6][7], 1.13:[6][4]
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
	return cisPocketSize[sizeX][sizeY];
}

void GetPocketDimensionsBySize(int pocketSize, int& sizeX, int& sizeY)
{
	static const UINT8 cisPocketSize[6][7] = // IoV 921-001:[6][7], 1.13:[6][4]
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
		for(sizeY=0; sizeY<7; sizeY++) // IoV 921-001:sizeY<7, 1.13:sizeY<4
		{
			if(pocketSize == cisPocketSize[sizeX][sizeY])
			{
				return;
			}
		}
	}
}

\\\\\\注释掉的是 1.13 的数值, 修改过的数值标记有 IoV


------------------------------------------------------------------------------------------
Tactical\Item Types.cpp
------------------------------------------------------------------------------------------

>>>>>>>>>>>>>>line 417

POCKETTYPE::POCKETTYPE(){
	memset(this, 0, SIZEOF_POCKETTYPE);
	ItemCapacityPerSize.resize(55); //IoV 921:55, 1.13:35
}

\\\\\\这里如果物品最大是54，那么就 54+1


>>>>>>>>>>>>>>line 421

POCKETTYPE::POCKETTYPE(const POCKETTYPE& src){
	memcpy(this, &src, SIZEOF_POCKETTYPE);
	ItemCapacityPerSize.resize(55); //IoV 921:55, 1.13:35
	ItemCapacityPerSize = src.ItemCapacityPerSize;
}

\\\\\\这里如果物品最大是54，那么就 54+1

------------------------------------------------------------------------------------------
Tactical\XML_LBEPocket.cpp
------------------------------------------------------------------------------------------

>>>>>>>>>>>>>>line 92

\\\\\\找到
strcmp(name, "ItemCapacityPerSize.34") == 0 || strcmp(name, "ItemCapacityPerSize34") == 0 ))
\\\\\\改为：

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

\\\\\\找到
		else if(strcmp(name, "ItemCapacityPerSize.34") == 0 || strcmp(name, "ItemCapacityPerSize34") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[34] = (UINT8) atol(pData->szCharData);
		}
\\\\\\在其下面增加：
		// IoV 921-001 add -->
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
		// IoV 921-001 add end <--



\\\\\\找到
FilePrintf(hFile,"\t\t<ItemCapacityPerSize34>%d</ItemCapacityPerSize34>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[34]   );
\\\\\\在其下面增加：

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
/Laptop/IMP Confirm.cpp 【重要，如果此处没有修改，那么IMP/AIM出场时大于34的物品无法携带】 by zwwooooo
------------------------------------------------------------------------------------------

>>>>>>>>>>>>>>line 513

	// Next sort list by size
	for(j=54; j>=0; j--) // IoV 921:54, 1.13:34 - ItemSize limit by zwwooooo
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

视距修改 by zwwooooo

===================================================================================================================

------------------------------------------------------------------------------------
Tactical\opplist.cpp
------------------------------------------------------------------------------------

>>>>>>>>>>>>>>line 239

\\\\\\找到如下

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

\\\\\\改为:

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

kenkenkenken 修改 http://tbsgame.net/bbs/index.php?showtopic=58400&view=findpost&p=813110

-JA2用榴弹换枪里的榴弹而不消耗AP的bug,

可以通过修改Tactical\Interface Items.cpp" from line 4694 to 4741.实现

(bp上的silversurfer如是说); 

TV：

这个问题我已经修复了，这段我重写了一下，解决了原程序如下问题：

1. 榴弹切换不消耗AP

2. 枪挂榴弹装弹 按装附件算AP 而不是 按装弹算AP

3. 加一个带弹的榴弹发射器到步枪上，会按装榴弹为附件算AP 而不是按装发射器为附件算AP ； 如果装一个没弹的榴弹发射器到步枪上，AP是对的

4. 从步枪上拆卸一个带弹的榴弹发射器，AP计算的bug与第3点类似，但不是一定会遇到，关键是卸载是附件格子里是 弹在前面 还是 发射器在前面，如果是弹在前面会遇到此bug

代码（包含被屏蔽的老代码）如下：

- 2011.10.28 代码已修正到最新版本4769（zwwooooo 注）

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
			// DBB's IoV---begin
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
			// DBB's IoV---end

			if (ubAPCost)
			{
				DeductPoints( gpAttachSoldier, ubAPCost, 0 );
			}
		}