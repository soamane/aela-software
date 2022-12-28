/*coded by https://github.com/soamane*/

#pragma once
#include "helpers.hpp"
static INT FloatU32(const FLOAT x)
{
	union { INT ui; FLOAT f; } cvt;
	cvt.f = x;

	return cvt.ui;
}
static bool NumberValid(const FLOAT& x)
{
	INT i = FloatU32(x);
	INT expmask = (0xFF << 23);
	INT iexp = i & expmask;
	BOOLEAN invalid = (iexp == expmask);

	if (invalid)
	{
		INT i = 0x7F800001;
		FLOAT fpe = *(FLOAT*)(&i);
	}

	return !invalid;
}

template <typename F> struct Vec3_tpl
{
	F x, y, z;
	Vec3_tpl(typeZero) : x(0), y(0), z(0) { }
	explicit Vec3_tpl(F f) : x(f), y(f), z(f) { }

	Vec3_tpl() { Vec3_tpl<F>(0, 0, 0); }
	Vec3_tpl(F xt, F yt, F zt) { x = xt; y = yt; z = zt; }
	void Set(F xt, F yt, F zt) { x = xt; y = yt; z = zt; }

	bool IsValid() const
	{
		if (!NumberValid(x)) return false;
		if (!NumberValid(y)) return false;
		if (!NumberValid(z)) return false;

		return true;
	}

	Vec3_tpl operator /  (F f) { return Vec3_tpl(x / f, y / f, z / f); }
	Vec3_tpl& operator = (const Vec3_tpl<F>& source) { x = source.x; y = source.y; z = source.z; return *this; }
	Vec3_tpl& operator = (const F value) { x = value; y = value; z = value;   return *this; }
	Vec3_tpl operator * (F f) { return Vec3_tpl(x * f, y * f, z * f); }
	Vec3_tpl& operator *= (F f) { x *= f; y *= f; z *= f; return *this; }
	Vec3_tpl& operator /= (F f) { x /= f; y /= f; z /= f; return *this; }
	bool operator==(const Vec3_tpl<F>& vec) { return this->x == vec.x && this->y == vec.y && this->z == vec.z; }
	bool operator!=(const Vec3_tpl<F>& vec) { return !(*this == vec); }

	Vec3_tpl& operator += (const Vec3_tpl<F>& vec) { x += vec.x; y += vec.y; z += vec.z; return *this; }
	Vec3_tpl& operator += (F f) { x += f; y += f; z += f; return *this; }

	friend Vec3_tpl operator + (const Vec3_tpl& vec1, const Vec3_tpl& vec2) { return Vec3_tpl(vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z); }
	friend Vec3_tpl operator + (const Vec3_tpl& vec1, const F* vec2) { return vec1 + Vec3_tpl(vec2); }
	friend Vec3_tpl operator + (const F* vec1, const Vec3_tpl& vec2) { return Vec3_tpl(vec1) + vec2; }

	friend Vec3_tpl operator - (const Vec3_tpl& vec1, const Vec3_tpl& vec2) { return Vec3_tpl(vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z); }
	friend Vec3_tpl operator - (const Vec3_tpl& vec1, const F* vec2) { return vec1 - Vec3_tpl(vec2); }
	friend Vec3_tpl operator - (const F* vec1, const Vec3_tpl& vec2) { return Vec3_tpl(vec1) - vec2; }

	Vec3_tpl<F> GetNormalized() const {
		FLOAT fInvLen = (FLOAT)nocrt::isqrt_safe_tpl(x * x + y * y + z * z);
		Vec3_tpl<F> out = *this;
		out.x *= fInvLen;
		out.y *= fInvLen;
		out.z *= fInvLen;
		return out;
	}
};
template <typename F> struct Vec4_tpl {
	F x, y, w, h;
	Vec4_tpl(typeZero) : x(0), y(0), w(0), h(0) { }
	explicit Vec4_tpl(F f) : x(f), y(f), w(f), h(f) { }

	Vec4_tpl() { Vec4_tpl<F>(0, 0, 0, 0); }
	Vec4_tpl(F xt, F yt, F wt, F ht) { x = xt; y = yt; w = wt; h = ht; }

	void Set(F xt, F yt, F wt, F ht) { x = xt; y = yt; w = wt; h = ht; }
};

template <typename F> struct Quat_tpl
{
	Vec3_tpl<F> v;
	F w;

	bool operator!=(const Quat_tpl<F>& q) const { return !(*this == q); }
	//Quat_tpl<F>	operator - () const { return Quat_tpl<F>(-w, -v); }; 
	Quat_tpl<F> operator - () const { //smooth
		Quat_tpl ret;
		ret.w = -w;
		ret.v.x = -v.x;
		ret.v.y = -v.y;
		ret.v.z = -v.z;
		return ret;
	};
	friend float operator | (const Quat_tpl& q, const Quat_tpl& p)
	{
		assert(q.v.IsValid());
		assert(p.v.IsValid());
		return (q.v.x * p.v.x + q.v.y * p.v.y + q.v.z * p.v.z + q.w * p.w);
	}
	friend float operator ==  (const Quat_tpl& q, const Quat_tpl& p) {
		return q.v.x == p.v.x && q.v.y == p.v.y && q.v.z == p.v.z;
	}
	friend Quat_tpl operator - (const Quat_tpl& q, const Quat_tpl& p)
	{
		Quat_tpl ret;
		ret.w = q.w - p.w;

		ret.v.x = q.v.x - p.v.x;
		ret.v.y = q.v.y - p.v.y;
		ret.v.z = q.v.z - p.v.z;

		return ret;
	}

	bool IsValid() const
	{
		if (!NumberValid(v.x)) return false;
		if (!NumberValid(v.y)) return false;
		if (!NumberValid(v.z)) return false;
		if (!NumberValid(w)) return false;

		return true;
	}

	void SetRotationVDir(const Vec3_tpl<F>& vdir) {
		w = (0.70710676908493042f);
		v.x = (vdir.z * 0.70710676908493042f);
		v.y = (0.0f);
		v.z = (0.0f);

		F l = (F)nocrt::_sqrt(vdir.x * vdir.x + vdir.y * vdir.y);

		if (l > (0.00001)) {
			Vec3_tpl<F> hv;
			hv.x = vdir.x / l;
			hv.y = vdir.y / l + 1.0f;
			hv.z = l + 1.0f;

			F r = (F)nocrt::_sqrt(hv.x * hv.x + hv.y * hv.y);
			F s = (F)nocrt::_sqrt(hv.z * hv.z + vdir.z * vdir.z);
			F hacos0 = 0.0;
			F hasin0 = -1.0;

			if (r > (0.00001)) {
				hacos0 = hv.y / r;
				hasin0 = -hv.x / r;
			}
			F hacos1 = hv.z / s;
			F hasin1 = vdir.z / s;
			w = (hacos0 * hacos1);
			v.x = (hacos0 * hasin1);
			v.y = (hasin0 * hasin1);
			v.z = (hasin0 * hacos1);
		}
	}

	static Quat_tpl<F> CreateRotationVDir(const Vec3_tpl<F>& vdir) {
		Quat_tpl<F> q;
		q.SetRotationVDir(vdir);
		return q;
	}

	void Normalize(void)
	{
		float d = nocrt::isqrt_tpl(w * w + v.x * v.x + v.y * v.y + v.z * v.z);

		w *= d;

		v.x *= d;
		v.y *= d;
		v.z *= d;
	}

	void SetNlerp(const Quat_tpl& p, const Quat_tpl& tq, float t)
	{
		Quat_tpl q = tq;

		assert(p.IsValid());
		assert(q.IsValid());

		if ((p | q) < 0)
		{
			float qx = -q.v.x;
			float qy = -q.v.y;
			float qz = -q.v.z;

			q.v.x = qx;
			q.v.y = qy;
			q.v.z = qz;
		}

		v.x = p.v.x * (1.0f - t) + q.v.x * t;
		v.y = p.v.y * (1.0f - t) + q.v.y * t;
		v.z = p.v.z * (1.0f - t) + q.v.z * t;

		w = p.w * (1.0f - t) + q.w * t;

		Normalize();
	}

	void SetSlerp(const Quat_tpl& tp, const Quat_tpl& tq, float t)
	{
		assert(tp.IsValid());

		Quat_tpl p, q;
		p = tp;
		q = tq;
		Quat_tpl q2;

		float cosine = (p | q);

		if (cosine < 0.0f)
		{
			float qx = -q.v.x;
			float qy = -q.v.y;
			float qz = -q.v.z;

			cosine = -cosine;

			q.v.x = qx;
			q.v.y = qy;
			q.v.z = qz;
		}

		if (cosine > 0.9999f)
		{
			SetNlerp(p, q, t);
			return;
		}

		q2.w = q.w - p.w * cosine;
		q2.v.x = q.v.x - p.v.x * cosine;
		q2.v.y = q.v.y - p.v.y * cosine;
		q2.v.z = q.v.z - p.v.z * cosine;

		float sine = nocrt::_sqrt(q2 | q2);
		float s, c;

		nocrt::sincos_tpl(atan2(sine, cosine) * t, &s, &c);

		w = (p.w * c + q2.w * s / sine);
		v.x = (p.v.x * c + q2.v.x * s / sine);
		v.y = (p.v.y * c + q2.v.y * s / sine);
		v.z = (p.v.z * c + q2.v.z * s / sine);
	}

	static Quat_tpl CreateSlerp(const Quat_tpl& p, const Quat_tpl& tq, float t)
	{
		Quat_tpl d;
		d.SetSlerp(p, tq, t);
		return d;
	}

	Vec3_tpl<F> GetForwardDir()
	{
		return Vec3_tpl<F>(2 * (v.x * v.y - v.z * w), 2 * (v.y * v.y + w * w) - 1, 2 * (v.z * v.y + v.x * w));
	}
};

template <typename F> struct QuatT_tpl
{
	Quat_tpl<F> q;
	Vec3_tpl<F> t;
};
template <typename F> struct Matrix33_tpl
{
	F m00, m01, m02;
	F m10, m11, m12;
	F m20, m21, m22;
};

template <typename F> struct Matrix44_tpl
{
	F m00, m01, m02, m03;
	F m10, m11, m12, m13;
	F m20, m21, m22, m23;
	F m30, m31, m32, m33;
};
template <typename F> struct Matrix34_tpl {
	F m00, m01, m02, m03;
	F m10, m11, m12, m13;
	F m20, m21, m22, m23;

	__forceinline Matrix34_tpl() {};
	template<class F1> explicit Matrix34_tpl(const QuatT_tpl<F1>& q) {
		Vec3_tpl<F1> v2 = q.q.v + q.q.v;
		F1 xx = 1 - v2.x * q.q.v.x;		F1 yy = v2.y * q.q.v.y;		F1 xw = v2.x * q.q.w;
		F1 xy = v2.y * q.q.v.x;			F1 yz = v2.z * q.q.v.y;		F1 yw = v2.y * q.q.w;
		F1 xz = v2.z * q.q.v.x;			F1 zz = v2.z * q.q.v.z;		F1 zw = v2.z * q.q.w;
		m00 = F(1 - yy - zz);			m01 = F(xy - zw);			m02 = F(xz + yw);			m03 = F(q.t.x);
		m10 = F(xy + zw);				m11 = F(xx - zz);			m12 = F(yz - xw);			m13 = F(q.t.y);
		m20 = F(xz - yw);				m21 = F(yz + xw);			m22 = F(xx - yy);			m23 = F(q.t.z);
	}
	__forceinline Vec3_tpl <F>GetTranslation() const { return Vec3_tpl <F>(m03, m13, m23); }

	__forceinline Matrix34_tpl <F>SetTranslation(Vec3_tpl<F>vPos) {
		m03 = F(vPos.x);
		m13 = F(vPos.y);
		m23 = F(vPos.z);
		return *this;
	}
};
template <typename F> struct HUDSilhouettesColor_tpl
{
	F r, g, b, a;
};
template <typename F> struct Color_tpl
{
public:
	F a, r, g, b;

	Color_tpl() { Color_tpl(0, 0, 0, 0); }
	Color_tpl(int a, int r, int g, int b) { this->a = a; this->r = r; this->g = g; this->b = b; }
	Color_tpl(int r, int g, int b) { this->a = 255; this->r = r; this->g = g; this->b = b; }
	Color_tpl(unsigned long color) { this->b = (color & 0xff); this->g = ((color >> 8) & 0xff); this->r = ((color >> 16) & 0xff); this->a = ((color >> 24) & 0xff); }
	inline float* Base()
	{
		float fColor[3];
		fColor[0] = this->r / 255.0f;
		fColor[1] = this->g / 255.0f;
		fColor[2] = this->b / 255.0f;
		return &fColor[0];
	}

	inline float rBase() const { return this->r / 255.0f; }
	inline float gBase() const { return this->g / 255.0f; }
	inline float bBase() const { return this->b / 255.0f; }
	inline float aBase() const { return this->a / 255.0f; }

	inline operator unsigned long() const { return (a << 24) | (r << 16) | (g << 8) | b; }
};
template <typename T> struct String_tpl
{
	typedef T					valueFpe;
	typedef const valueFpe* const_str;
	typedef const valueFpe* const_pointer;
	typedef const_pointer		const_iterator;

	struct StrHeader
	{
		volatile int nRefCount;
		int nLength;
		int nAllocSize;
		valueFpe* GetChars() { return (valueFpe*)(this + 1); }
		void AddRef() { ++nRefCount; };
		int Release() { return --nRefCount; };
	};

	int length() const { return m_header->nLength; }

	const_iterator begin() const { return m_str; };
	const_iterator end() const { return m_str + length(); };

	operator const_str() const { return m_str; }

	const valueFpe* c_str() const { return m_str; }
	const valueFpe* data() const { return m_str; };

	valueFpe* m_str;
	StrHeader* m_header;
};


template <class TVarType, typename TMaskType = uint32_t, TMaskType TMaxMaskValue = 0x7FFFFFFF>
class CMaskedVarT
{
public:
	CMaskedVarT() : m_mask(0) { }
	CMaskedVarT(const TVarType v) { Set(v); }
	operator TVarType () const { return this->Get(); }
	CMaskedVarT& operator += (const TVarType rhs)
	{
		TVarType  v = this->Get();
		v += rhs;
		this->Set(v);
		return (*this);
	}
	CMaskedVarT& operator -= (const TVarType rhs)
	{
		TVarType  v = this->Get();
		v -= rhs;
		this->Set(v);
		return (*this);
	}
	CMaskedVarT& operator *= (const TVarType rhs)
	{
		TVarType  v = this->Get();
		v *= rhs;
		this->Set(v);
		return (*this);
	}
	CMaskedVarT& operator /= (const TVarType rhs)
	{
		TVarType  v = this->Get();
		v /= rhs;
		this->Set(v);
		return (*this);
	}
	CMaskedVarT& operator = (const TVarType rhs)
	{
		Set(rhs);
		return (*this);
	}
private:
	void Set(const TVarType v)
	{
		UVal		w;
		w.asVarType = v;
		m_val.asMaskType = (w.asMaskType ^ m_mask);
	}
	TVarType Get() const
	{
		UVal  w;
		w.asMaskType = 0;
		if (m_mask > 0) w.asMaskType = (m_val.asMaskType ^ m_mask);
		return w.asVarType;
	}
	union UVal
	{
		TVarType  asVarType;
		TMaskType  asMaskType;
	};
	UVal  m_val;
	TMaskType  m_mask;
};
using TMaskedFloat = CMaskedVarT<float>;

template <class F, class F2> Vec3_tpl<F> operator*(const QuatT_tpl<F>& q, const Vec3_tpl<F2>& v) {
	Vec3_tpl<F> out, r2;
	r2.x = (q.q.v.y * v.z - q.q.v.z * v.y) + q.q.w * v.x;
	r2.y = (q.q.v.z * v.x - q.q.v.x * v.z) + q.q.w * v.y;
	r2.z = (q.q.v.x * v.y - q.q.v.y * v.x) + q.q.w * v.z;
	out.x = (r2.z * q.q.v.y - r2.y * q.q.v.z);
	out.x += out.x + v.x + q.t.x;
	out.y = (r2.x * q.q.v.z - r2.z * q.q.v.x);
	out.y += out.y + v.y + q.t.y;
	out.z = (r2.y * q.q.v.x - r2.x * q.q.v.y);
	out.z += out.z + v.z + q.t.z;
	return out;
}
template <class F> Vec3_tpl<F> operator*(const Matrix34_tpl<F>& m, const Vec3_tpl<F>& p) {
	Vec3_tpl<F> tp;
	tp.x = m.m00 * p.x + m.m01 * p.y + m.m02 * p.z + m.m03;
	tp.y = m.m10 * p.x + m.m11 * p.y + m.m12 * p.z + m.m13;
	tp.z = m.m20 * p.x + m.m21 * p.y + m.m22 * p.z + m.m23;
	return tp;
}
template <class F> Matrix34_tpl<F> operator * (const Matrix34_tpl<F>& l, const Matrix34_tpl<F>& r) {
	Matrix34_tpl<F> m;
	m.m00 = l.m00 * r.m00 + l.m01 * r.m10 + l.m02 * r.m20;
	m.m10 = l.m10 * r.m00 + l.m11 * r.m10 + l.m12 * r.m20;
	m.m20 = l.m20 * r.m00 + l.m21 * r.m10 + l.m22 * r.m20;
	m.m01 = l.m00 * r.m01 + l.m01 * r.m11 + l.m02 * r.m21;
	m.m11 = l.m10 * r.m01 + l.m11 * r.m11 + l.m12 * r.m21;
	m.m21 = l.m20 * r.m01 + l.m21 * r.m11 + l.m22 * r.m21;
	m.m02 = l.m00 * r.m02 + l.m01 * r.m12 + l.m02 * r.m22;
	m.m12 = l.m10 * r.m02 + l.m11 * r.m12 + l.m12 * r.m22;
	m.m22 = l.m20 * r.m02 + l.m21 * r.m12 + l.m22 * r.m22;
	m.m03 = l.m00 * r.m03 + l.m01 * r.m13 + l.m02 * r.m23 + l.m03;
	m.m13 = l.m10 * r.m03 + l.m11 * r.m13 + l.m12 * r.m23 + l.m13;
	m.m23 = l.m20 * r.m03 + l.m21 * r.m13 + l.m22 * r.m23 + l.m23;
	return m;
}
template <class CVirtualIndex> CVirtualIndex CallFunction(PVOID pVoid, uintptr_t Index) {
	return (*(CVirtualIndex**)pVoid)[Index / 8];
}
template <class CVirtualAddres> CVirtualAddres CallFunction(uintptr_t Address) {
	return (CVirtualAddres)(Address);
}
typedef Vec3_tpl<float>								Vec3;
typedef Matrix34_tpl<float>							Matrix34;
typedef Quat_tpl<float>								Quat;
typedef QuatT_tpl<float>							QuatT;


//13,78,71,25,48,70,77,4,5

enum EBone : UINT32
{
	Bone_Pelvis = 1,
	Bone_Locomotion,
	Bone_Spine,
	Bone_L_Thigh,
	Bone_R_Thigh,
	Bone_Groin_Back,
	Bone_Groin_Front,
	Bone_Spine1,
	Bone_Spine2,
	Bone_Spine3,
	Bone_Wep_Law,
	Bone_Neck,
	Bone_Head,
	Bone_L_Clavicle,
	Bone_R_Clavicle,
	Bone_L_Eye,
	Bone_R_Eye,
	Bone_Camera,
	Bone_L_Eye_01,
	Bone_R_Eye_01,
	Bone_HNeck,
	Bone_Camera_01,
	Bone_HNeck_End,
	Bone_L_UpperArm,
	Bone_L_Forearm,
	Bone_L_Hand,
	Bone_L_ForeTwist,
	Bone_L_Finger0,
	Bone_L_Finger1,
	Bone_L_Finger2,
	Bone_L_Finger3,
	Bone_L_Finger4,
	Bone_Wep_Alt,
	Bone_L_Hand_Push,
	Bone_L_Finger01,
	Bone_L_Finger02,
	Bone_L_Finger11,
	Bone_L_Finger12,
	Bone_L_Finger21,
	Bone_L_Finger22,
	Bone_L_Finger31,
	Bone_L_Finger32,
	Bone_L_Finger41,
	Bone_L_Finger42,
	Bone_L_ForeTwist_1,
	Bone_L_ForeTwist_2,
	Bone_R_UpperArm,
	Bone_R_Forearm,
	Bone_R_Hand,
	Bone_R_ForeTwist,
	Bone_R_Finger0,
	Bone_R_Finger1,
	Bone_R_Finger2,
	Bone_R_Finger3,
	Bone_R_Finger4,
	Bone_WepBone,
	Bone_R_Hand_Push,
	Bone_R_Finger01,
	Bone_R_Finger02,
	Bone_R_Finger11,
	Bone_R_Finger12,
	Bone_R_Finger21,
	Bone_R_Finger22,
	Bone_R_Finger31,
	Bone_R_Finger32,
	Bone_R_Finger41,
	Bone_R_Finger42,
	Bone_R_ForeTwist_1,
	Bone_R_ForeTwist_2,
	Bone_L_Calf,
	Bone_L_Foot,
	Bone_L_Toe0,
	Bone_L_Heel,
	Bone_L_Heel01,
	Bone_L_Toe0Nub,
	Bone_L_Toe0Nub01,
	Bone_R_Calf,
	Bone_R_Foot,
	Bone_R_Toe0,
	Bone_R_Heel,
	Bone_R_Heel01,
	Bone_R_Toe0Nub,
	Bone_R_Toe0Nub01,
	Bone_Groin_Back_End,
	Bone_Groin_Front_End,
	Bone_Locator_Collider
};


