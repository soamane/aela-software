/*coded by https://github.com/soamane*/

#include "stdafx.hpp"
#include "math.h"
#include <map>

#define RELEASE_OFFSETS(TYPE, NAME, OFFSET) \
TYPE NAME##_GET() { \
return *reinterpret_cast< TYPE* >(this + OFFSET); \
} \
\
void NAME##_SET(TYPE VALUE) { \
*reinterpret_cast< TYPE* >(this + OFFSET) = VALUE; \
} \
__declspec(property(get = NAME##_GET, put = NAME##_SET)) TYPE NAME \

class CSkeletonPose {
	public:
	QuatT_tpl<float> &GetBoneById( int id ) {
		return vtable::vtable<0x38 / 8, QuatT_tpl<float> &>( this, id );
	}
};

class CCharacterInstance {
	public:
	CSkeletonPose *GetSketetonPose( ) {
		return vtable::vtable<0x28 / 8, CSkeletonPose *>( this );
	}
};

struct CPhysicalentity {
	public:
	void *GetForeignData( int itype = 0 ) {
		return vtable::vtable<0x68 / 8, void *>( this, itype );
	}
};


class CRenderer {
	private:
	struct SWorldToScreen {
		public:
		float toX;
		float toY;
		float toZ;
		float *outX;
		float *outY;
		float *outZ;
	};
	bool ProjectToScreenInternal( SWorldToScreen *proj ) {
		return vtable::vtable<112, bool >( this, proj );
	}
	int GetWidth( ) {
		return vtable::vtable<0x330 / 8, int>( this );
	}
	int GetHeight( ) {
		return  vtable::vtable<0x338 / 8, int>( this );
	}
	public:
	RELEASE_OFFSETS( Vec3_tpl<float>, GetViewCamera, 0x17C0 );
	RELEASE_OFFSETS( IDXGISwapChain *, GetSwapChain, 0x14D000 );

	bool WorldToScreen( Vec3_tpl<float> to, Vec3_tpl<float> &out ) {

		SWorldToScreen w2s;

		w2s.toX = to.x;
		w2s.toY = to.y;
		w2s.toZ = to.z;
		w2s.outX = &out.x;
		w2s.outY = &out.y;
		w2s.outZ = &out.z;
		ProjectToScreenInternal( &w2s );

		out.x *= ( GetWidth( ) * 0.01f );
		out.y *= ( GetHeight( ) * 0.01f );
		out.z *= 1.0f;

		return ( ( out.z < 1.0f ) && ( out.x > 0 ) && ( out.x < GetWidth( ) ) && ( out.y > 0 ) && ( out.y < GetHeight( ) ) );

	}

	using FN_DrawText = void( __thiscall * )( CRenderer *, float, float, float, const float *, bool, const char * );
	void DrawTextA( float x, float y, float font_size, const float *col, bool center, const char *label_text, ... ) {
		return FN_DrawText( 0x14020FB00 )( this, x, y, font_size, col, center, label_text );//48 8B C4 48 83 EC 68 F3 0F 11 58
	}
};

class CEntity {

	public:
	RELEASE_OFFSETS( Matrix34_tpl<float >, GetEntityWorldTM, 0x40 );

	CCharacterInstance *GetCharacterInstance( int number ) {
		return vtable::vtable<0x298 / 8, CCharacterInstance *>( this, number );
	}

	UINT GetEntityId( ) {
		return vtable::vtable<1, UINT>( this );
	}

	Vec3_tpl<float> &GetPos( ) {
		return vtable::vtable<0x1C, Vec3_tpl<float> &>( this );
	}

	const char *GetName( ) {
		return vtable::vtable<0x60 / 8, const char *>( this );
	}

	Vec3_tpl<float> GetBonePos( CSkeletonPose *pLocalSkeleton, int eBone ) {
		Matrix34_tpl<float> World = GetEntityWorldTM;
		Matrix34_tpl<float> SkeletonAbs = Matrix34_tpl<float>( pLocalSkeleton->GetBoneById( eBone ) );
		Matrix34_tpl<float> matWorld = World;

		float m03, m13, m23;
		m03 = ( World.m00 * SkeletonAbs.m03 ) + ( World.m01 * SkeletonAbs.m13 ) + ( World.m02 * SkeletonAbs.m23 ) + World.m03;
		matWorld.m03 = m03;
		m13 = ( World.m10 * SkeletonAbs.m03 ) + ( World.m11 * SkeletonAbs.m13 ) + ( World.m12 * SkeletonAbs.m23 ) + World.m13;
		matWorld.m13 = m13;
		m23 = ( World.m20 * SkeletonAbs.m03 ) + ( World.m21 * SkeletonAbs.m13 ) + ( World.m22 * SkeletonAbs.m23 ) + World.m23;
		matWorld.m23 = m23;

		return matWorld.GetTranslation( );
	}

};

class CEntityIterator {
	public:

	CEntity *Iterator( ) {
		return vtable::vtable<6, CEntity *>( this );
	}
};

class CEntitySystem {
	public:
	CEntity *GetEntityPhysics( CPhysicalentity *pPhysEntity ) {
		return vtable::vtable<26, CEntity *>( this, pPhysEntity );
	}
	CEntityIterator *GetEntityIterator( ) {
		return vtable::vtable <21, CEntityIterator * >( this );
	}
};

struct CWeaponParameters {
	public:

	void SetValue( UINT type, float value ) {

		auto it = m_stats.find( type );
		if ( it != m_stats.end( ) ) {
			it->second = value;
		}
	}


	private:
	typedef std::map<short, CMaskedVarT<float, unsigned int>> TStatsData;
	char pad_0x00[0x68];
	TStatsData m_stats;

};

class CWeaponNetwork {
	public:

};

class CWeaponGeneral {
	public:

	using FN_UseDefibrillator = void( __thiscall * )( CWeaponGeneral *, unsigned int, bool );
	void UseDefibrillator( unsigned int targetId, bool resurrect ) {
		return FN_UseDefibrillator( 0x141490550 )( this, targetId, resurrect );
	}

	RELEASE_OFFSETS( CWeaponNetwork *, GetWeaponNetwork, 0x50 );
	RELEASE_OFFSETS( UINT, GetWeaponStateOffset, 0x110 );

	CWeaponParameters *GetWeaponParameters( ) {
		return vtable::vtable<2, CWeaponParameters *>( this );
	}

	UINT GetWeaponState( ) {
		return vtable::vtable<5, UINT>( this );
	}

	bool IsZooming( ) {
		UINT m_currState = this->GetWeaponState( );
		return (unsigned int)( m_currState - 22 ) <= 9 || (unsigned int)( m_currState - 16 ) <= 1;
	}

	bool IsZoomed( ) {
		UINT m_currState = this->GetWeaponState( );
		if ( m_currState == 2 )
			return true;
	}

	void BoltAction( bool need ) {
		*reinterpret_cast<bool *>( this + 0xd4 ) = need;
	}

};

class CWeapon {
	public:
	RELEASE_OFFSETS( CWeaponGeneral *, GetWeaponGeneral, 0x8 );

	void StartShooting( ) {
		return vtable::vtable<11, void>( this );
	}
	void StopShooting( ) {
		return vtable::vtable<12, void>( this );
	}
};

class CItem {
	public:

	CWeapon *GetWeapon( ) {
		return vtable::vtable<80, CWeapon *>( this );
	}

};

class CWorldQuery {
	public:

	using FN_GetLookAtEntity = UINT( __thiscall * )( CWorldQuery *, bool );
	UINT LookAtEntity( bool ignore_glass ) {
		return FN_GetLookAtEntity( 0x1405AA510 )( this, ignore_glass );//83 4B 28 01 40 84
	}
	UINT GetLookAtEntityId( ) {
		return vtable::vtable<28, UINT>( this );//vtable 28index
	}
};

struct CGameObject {
	CWorldQuery *GetWorldQuery( ) {
		auto offset_1 = *reinterpret_cast<UINT *>( this + ( 0x68 ) );//83 4B 28 01 40 84
		if ( !ptrs::valid_pointer( offset_1 ) ) {
			return nullptr;
		}
		return *reinterpret_cast<CWorldQuery **>( offset_1 + ( 0x20 ) );;
	}
};

class CActor {
	public:

	RELEASE_OFFSETS( CGameObject *, GetGameObject, 0x18 );
	RELEASE_OFFSETS( UINT, GetCurrentClass, 1112 );
	RELEASE_OFFSETS( UINT, GetTeam, 992 );
	RELEASE_OFFSETS( UINT, GetEntityId, 0x20 );

	UINT GetHealth( ) {
		return vtable::vtable<28, int>( this );
	}

	UINT GetArmor( ) {
		return vtable::vtable<33, int>( this );
	}

	const BOOL IsDead( ) {
		return GetHealth( ) <= 0;
	}

	CItem *GetCurrentItem( int item = 0 ) {
		return vtable::vtable<58, CItem *>( this, item );
	}

	void SetThirdPerson( bool bValue ) {
		vtable::vtable< 67, void >( this, bValue );
	}
};

class CActorSystem {
	public:
	CActor *GetActor( UINT entity_id ) {
		return vtable::vtable<15, CActor *>( this, entity_id );
	}
};

class CGameFramework {
	public:
	bool GetClientActor( CActor **mActor ) {
		return vtable::vtable<142, bool>( this, mActor );
	}
	CActorSystem *GetActorSystem( ) {
		return vtable::vtable<25, CActorSystem *>( this );
	}

};
class CSVars {
	public:
	RELEASE_OFFSETS( UINT, g_victory_condition, 0x638 );

	bool IsInRoom( INT Room ) {
		return this->g_victory_condition == Room;
	}

};

class CGame {
	public:
	RELEASE_OFFSETS( CSVars *, GetCSvars, 0x130 );
	CGameFramework *GetFramework( ) {
		return vtable::vtable<15, CGameFramework *>( this );
	}
};

class CTimer {
	private:
	enum ETimer {
		ETIMER_GAME,
		ETIMER_UI,
		ETIMER_LAST,
	};
	public:
	float GetFrameTime( ETimer which = ETIMER_GAME ) {
		return vtable::vtable<0x7, float>( this, which );
	}
};

class CSystemGlobalEnvironment {
	public:
	class CSystem {};

	RELEASE_OFFSETS( CSystem *, GetSystem, 0xF0 );
	RELEASE_OFFSETS( CRenderer *, GetRenderer, 0x48 );
	RELEASE_OFFSETS( CEntitySystem *, GetEntitySystem, 0xe0 );
	RELEASE_OFFSETS( CGame *, GetGame, 0xd0 );
	RELEASE_OFFSETS( CTimer *, GetTimer, 0xC0 );

	static CSystemGlobalEnvironment *Instance( ) {
		return *reinterpret_cast<CSystemGlobalEnvironment **>( 0x14221CF00 );
	}
};