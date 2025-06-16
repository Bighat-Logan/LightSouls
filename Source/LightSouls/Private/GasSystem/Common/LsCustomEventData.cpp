#include "GasSystem/Common/LsCustomEventData.h"

ULsCustomEventData::ULsCustomEventData()
{
    VectorValue = FVector::ZeroVector;
}

ULsCustomEventData* ULsCustomEventData::MakeLsCustomEventData()
{
    return NewObject<ULsCustomEventData>();
} 