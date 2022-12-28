/*coded by https://github.com/soamane*/

enum typeZero {
	ZERO
};

namespace vtable
{
	template  <uintptr_t template_index, typename template_type, typename... Args>
	template_type vtable(void* state_called, Args... args)
	{
		using fnTable = template_type(__fastcall*)(void*, Args...);
		auto  function = (*reinterpret_cast<fnTable**>(state_called))[(template_index * 0xFFFFF) / 0xFFFFF];
		return  function(state_called, args...);
	}

	template <typename TAddress>
	TAddress a_vtable( uintptr_t address ) {
		return (TAddress)( address );
	}
}

namespace nocrt
{
	char* str_cat(char str1[], char str2[])
	{
		int i = 0, j = 0;
		while (str1[i] != '\0')
			i++;
		while (str2[j] != '\0')
		{
			str1[i] = str2[j];
			j++;
			i++;
		}
		str1[i] = '\0';
		return str1;
	}

	char* __itoa(int number, char* destination, int base)
	{
		int count = 0;
		do
		{
			int digit = number % base;
			destination[count++] = (digit > 9) ? digit - 10 + 'A' : digit + '0';
		} while ((number /= base) != 0);
		destination[count] = 0;
		int i;
		for (i = 0; i < count / 2; ++i)
		{
			wchar_t symbol = destination[i];
			destination[i] = destination[count - i - 1];
			destination[count - i - 1] = symbol;
		}
		return destination;
	}

	static float _sqrt(float number) {
		long i;
		float x2, y;
		const float threehalfs = 1.5F;

		x2 = number * 0.5F;
		y = number;
		i = *(long*)&y;                     // floating point bit level hacking [sic]
		i = 0x5f3759df - (i >> 1);             // Newton's approximation
		y = *(float*)&i;
		y = y * (threehalfs - (x2 * y * y)); // 1st iteration
		y = y * (threehalfs - (x2 * y * y)); // 2nd iteration
		y = y * (threehalfs - (x2 * y * y)); // 3rd iteration

		return 1 / y;
	}
	size_t _strlen(const char* string) {
		int count = 0;
		if (string) {
			for (; *string != 0; ++string) count++;
		}
		return count;
	}

	void* __cdecl _memset(void* dest, int value, size_t num)
	{
		__stosb(static_cast<unsigned char*>(dest),
			static_cast<unsigned char>(value), num);
		return dest;
	}

	void* __cdecl _memcpy(void* dest, const void* src, size_t num)
	{
		__movsb(static_cast<unsigned char*>(dest),
			static_cast<const unsigned char*>(src), num);
		return dest;
	}

	void __fastcall zero_mem(LPVOID a1, DWORD64 a2) {
		_memset((a1), NULL, (a2));
	}


	static void  sincos_tpl(float angle, float* pSin, float* pCos) { *pSin = float(sin(angle));  *pCos = cos(angle); }
	static void  sincos_tpl(double angle, double* pSin, double* pCos) { *pSin = double(sin(angle));  *pCos = double(cos(angle)); }
	static double isqrt_tpl(double op) { return 1.0 / _sqrt(op); }
	static double isqrt_safe_tpl(double op) { return 1.0 / _sqrt(op + DBL_MIN); }
}



namespace hooks {
	struct ivmt
	{
	public:

		void setup_hook(void* baseclass, int size = 1024)
		{
			this->baseclass = static_cast<uintptr_t**>(baseclass);
			if (size == NULL)
				return;
			const size_t table_size = size * sizeof uintptr_t;
			nocrt::_memcpy(this->allocate, *reinterpret_cast<void**>(this->baseclass), table_size);
			this->original_vft = *reinterpret_cast<void**>(this->baseclass);
			*reinterpret_cast<void**>(this->baseclass) = this->allocate;
		};

		//void disable()
		//{
		//	*reinterpret_cast<void**>(this->baseclass) = this->original_vft;
		//}

		void* hk_function(int index, void* function)
		{
			*reinterpret_cast<void**>((uintptr_t*)this->allocate + index) = function;
			return *reinterpret_cast<void**>((uintptr_t*)this->original_vft + index);
		}
		//void clear()
		//{
		//	if (this->allocate)
		//	{
		//		nocrt::zero_mem(this->allocate, 8 * 1024);
		//		this->baseclass = nullptr;
		//		this->original_vft = nullptr;
		//	}
		//}

		//void shutdown() {
		//	this->clear();
		//	this->disable();
		//}


	private:
		void* baseclass = nullptr;
		void* allocate[1024];
		void* original_vft = nullptr;
	};
}



float green[4] = { 0.f, 1.f, 0.f, 1.f };
float blue[4] = { 0.f, 0.f, 1.f, 1.f };
float white[4] = { 1.f, 1.f, 1.f, 1.f };
float red[4] = { 1.f, 0.f, 0.f, 1.f };
float yellow[4] = { 0.8f, 1.f, 0.15f, 1.f };
namespace ptrs {

	template<typename t>
	inline auto valid_pointer(t pointer) -> bool {
		return (pointer && pointer > (t)0x400000 && pointer < (t)0x7FFFFFFFFFFF);
	}
}


enum EGameStatType {

	eGS_Absorption = 0x0,
	eGS_Accuracy = 0x1,
	eGS_ActivateDuration = 0x2,
	eGS_AimAccuracy = 0x3,
	eGS_AiSoundRadius = 0x4,
	eGS_AmmoAmount = 0x5,
	eGS_ArmorHealth = 0x6,
	eGS_ArmorRegenHps = 0x7,
	eGS_ArmorRegenTimeout = 0x8,
	eGS_ArmsDamageMult = 0x9,
	eGS_AutoZoomIn = 0x0A,
	eGS_AutoZoomOut = 0x0B,
	eGS_BallisticIronsight = 0x0C,
	eGS_BayonetMeleeDuration = 0x0D,
	eGS_BayonetMeleeOutDuration = 0x0E,
	eGS_BlindAmountMul = 0x0F,
	eGS_BodyDamageMult = 0x10,
	eGS_BoltAction = 0x11,
	eGS_BoltActionDuration = 0x12,
	eGS_BulletPierceability = 0x13,
	eGS_ClaymoreActivationDelayMul = 0x14,
	eGS_ClaymoreDetectorRadius = 0x15,
	eGS_ClipSize = 0x16,
	eGS_CrouchSpeedMul = 0x17,
	eGS_CustomizeInDuration = 0x18,
	eGS_CustomizeOutDuration = 0x19,
	eGS_Damage = 0x1A,
	eGS_DamageDropMinDistance = 0x1B,
	eGS_DamageDropPerMeter = 0x1C,
	eGS_DamageMin = 0x1D,
	eGS_DamageUnderbarrel = 0x1E,
	eGS_DeselectDuration = 0x1F,
	eGS_DeselectDurationBuff = 0x20,
	eGS_ExtraAmmoPistolMul = 0x21,
	eGS_ExtraAmmoPrimaryMul = 0x22,
	eGS_FireUnderbarrelDuration = 0x23,
	eGS_FireUnderbarrelOnRelease = 0x24,
	eGS_FlashbangTimeMul = 0x25,
	eGS_FovMul = 0x26,
	eGS_HeadDamageMult = 0x27,
	eGS_HealthMul = 0x28,
	eGS_HipAccuracy = 0x29,
	eGS_HolsterInDuration = 0x2A,
	eGS_HpRegenerationHps = 0x2B,
	eGS_HpRegenerationTimeout = 0x2C,
	eGS_IgnoreBulletBody = 0x2D,
	eGS_SuppressIgnoreBulletBody = 0x2E,
	eGS_IgnoreBulletHead = 0x2F,
	eGS_SuppressIgnoreBulletHead = 0x30,
	eGS_KnockBackChance = 0x31,
	eGS_KnockBackMinDistance = 0x32,
	eGS_KnockBackMaxDistance = 0x33,
	eGS_KnockBackResistance = 0x34,
	eGS_KnockBackStaminaDamage = 0x35,
	eGS_LegsDamageMult,

	eGS_Unknown0,
	eGS_Unknown1,

	eGS_MeleeDamage,
	eGS_MeleeDuration,
	eGS_MeleeDurationBuff,
	eGS_MeleeKnockBackChance,
	eGS_MeleeKnockBackStaminaDamage,
	eGS_CyborgDamageBuff,
	eGS_MeleeOutDuration,
	eGS_MeleeRadius,
	eGS_MeleeRange,
	eGS_MeleeRes,
	eGS_Range,

	eGS_RclAtt,
	eGS_RclDec,
	eGS_RclDecDelay,
	eGS_RclMax,
	eGS_RclRnd,
	eGS_RclSmth,
	eGS_ReloadDuration,
	eGS_ReloadDurationBuff,
	eGS_ReloadUnderbarrelDuration,
	eGS_ResArms,
	eGS_ResBody,
	eGS_ResHead,
	eGS_ResLegs,
	eGS_Resistance,
	eGS_Rpm,
	eGS_MoveSoundVolumeDamping,
	eGS_SecondaryMeleeDamage,
	eGS_SecondaryMeleeDuration,
	eGS_SecondaryMeleeDurationBuff,
	eGS_SecondaryMeleeOutDuration,
	eGS_SecondaryMeleeRadius,
	eGS_SecondaryMeleeRange,
	eGS_SelectDuration,
	eGS_SelectDurationBuff,
	eGS_SlideDistMul,
	eGS_SplashRes,

	eGS_SprdAtt,
	eGS_SprdDec,
	eGS_SprdDecDelay,
	eGS_SprdMax,
	eGS_SprdMin,

	eGS_SprintSpeedMul,
	eGS_SprintTimeMul,
	eGS_StaminaMul,
	eGS_SwayDec,
	eGS_SwayMinX,
	eGS_SwayMinY,
	eGS_SwaySpeedMult,
	eGS_ThrowDelayDuration,
	eGS_ThrowDuration,
	eGS_ThrowFireDelayDuration,
	eGS_TossDuration,
	eGS_ZoomFactor,
	eGS_ZoomInDuration,
	eGS_ZoomInDurationBuff,
	eGS_ZoomOutDuration,
	eGS_ZoomOutDurationBuff,
	eGS_ZoomedBoltActionDuration,
	eGS_PoisonGrenadeDamageMul,
	eGS_InSlotSelectDuration,
	eGS_InSlotDeselectDuration,
	eGS_UseInSlotWeaponChange,
	eGS_CameraShakeMul,
	eGS_SprdInverted,
	eGS_StrafeMultiplier,
	eGS_ClimbExtraHeight,
	eGS_ArmorAbsorbRatio,
	eGS_InvalidStat = -1 // 0x0FFFFFFFF
};

