#pragma once

#include <g3types.h>

/*
 * VectorUtil contains a set of utility functions for matrix/vector math.
 */


namespace g3
{

	/*
	 * if bToZAxis is false, compute matrix that rotates Z axis into vAlignWith
	 * if bToZAxis is true, compute matrix that rotates vAlignWith into Z axis
	 */
	template <class Real>
	void ComputeAlignZAxisMatrix( const Vector3<Real> & vAlignWith,
											  Matrix3<Real> & matrix, bool bToZAxis = false );

	template <class Real>
	void ComputeAlignAxisMatrix( const Vector3<Real> & vInitial,
								 const Vector3<Real> & vAlignWith, Matrix3<Real> & matrix );

	//! compute vectors in a plane perpendicular to vIn
	template <class Real>
	void ComputePerpVectors( const Vector3<Real> & vIn,
							 Vector3<Real> & vOut1, Vector3<Real> & vOut2,
							 bool bInIsNormalized = false);

	//! compute tangent vectors in plane perp to vNormal, using non-orthogonal vEstX as estimate of vOut1
	template <class Real>
	void ComputePerpVectors( const Vector3<Real> & vNormal,  const Vector3<Real> & vEstX,
							 Vector3<Real> & vOut1, Vector3<Real> & vOut2,
							 bool bInputIsNormalized = false);


	template <class Real>
	void ToGLMatrix( const Matrix3<Real> & matrix, Real glMatrix[16] );

	template<class Real>
	Vector2<Real> ToUV( const Vector3<Real> & vec, int nUIndex, int nVIndex );
	template<class Real>
	Vector3<Real> To3D( const Vector2<Real> & vec, int nUIndex, int nVIndex );


	template <class Real>
	Real VectorAngle( const Vector2<Real> & v1, const Vector2<Real> & v2 );
	template <class Real>
	Real VectorAngle( const Vector3<Real> & v1, const Vector3<Real> & v2 );

	template <class Real>
	Real VectorCot( const Vector3<Real> & v1, const Vector3<Real> & v2 );

	template <class Real>
	void BarycentricCoords( const Vector3<Real> & vTriVtx1, 
										const Vector3<Real> & vTriVtx2,
										const Vector3<Real> & vTriVtx3,
										const Vector3<Real> & vVertex,
										Real & fBary1, Real & fBary2, Real & fBary3 );

	template <class Real>
	Real Area( const Vector3<Real> & vTriVtx1, 
						   const Vector3<Real> & vTriVtx2,
						   const Vector3<Real> & vTriVtx3 );

	template <class Real>
	void BarycentricCoords( const Vector2<Real> & vTriVtx1, 
										const Vector2<Real> & vTriVtx2,
										const Vector2<Real> & vTriVtx3,
										const Vector2<Real> & vVertex,
										Real & fBary1, Real & fBary2, Real & fBary3 );

	template <class Real>
	Real Area( const Vector2<Real> & vTriVtx1, 
						   const Vector2<Real> & vTriVtx2,
						   const Vector2<Real> & vTriVtx3 );


	template <class Real>
	Vector3<Real> Normal( const Vector3<Real> & vTriVtx1, 
						  const Vector3<Real> & vTriVtx2,
						  const Vector3<Real> & vTriVtx3, Real * pArea = nullptr );


	template <class Real>
	Vector3<Real> InterpNormal( const Vector3<Real> & vTriVtx1, 
									 const Vector3<Real> & vTriVtx2,
									 const Vector3<Real> & vTriVtx3, 
									 const Vector3<Real> & vTriNorm1, 
									 const Vector3<Real> & vTriNorm2,
									 const Vector3<Real> & vTriNorm3,
									 const Vector3<Real> & vPointInTri );
	


	//! This metric is from Texture Mapping Progressive Meshes, Sander et al, Siggraph 2001
	template <class Real>
	void StretchMetric1( const Vector3<Real> & vTriVtx1, 
									 const Vector3<Real> & vTriVtx2,
									 const Vector3<Real> & vTriVtx3,
									 const Vector2<Real> & vVtxParam1,
									 const Vector2<Real> & vVtxParam2,
									 const Vector2<Real> & vVtxParam3,
									 Real & MaxSV, Real & MinSV, Real & L2Norm, Real & LInfNorm );

	template <class Real>
	void StretchMetric3( const Vector3<Real> & vTriVtx1, 
									 const Vector3<Real> & vTriVtx2,
									 const Vector3<Real> & vTriVtx3,
									 const Vector3<Real> & vVtxParam1,
									 const Vector3<Real> & vVtxParam2,
									 const Vector3<Real> & vVtxParam3,
									 Real & MaxSV, Real & MinSV, Real & L2Norm, Real & LInfNorm );


	template <class Real>
	bool IsObtuse( const Vector2<Real> & v1, const Vector2<Real> & v2, const Vector2<Real> & v3 );
	template <class Real>
	bool IsObtuse( const Vector3<Real> & v1, const Vector3<Real> & v2, const Vector3<Real> & v3 );



	/*
	 * inline utilities
	 */

	inline Vector2f d2f(const Vector2d & v) { 
		return Vector2f((float)v[0], (float)v[1]); 
	}
	inline Vector3f d2f(const Vector3d & v) {
		return Vector3f((float)v[0], (float)v[1], (float)v[2]); 
	}
	inline Vector2d f2d(const Vector2f & v) {
		return Vector2d((double)v[0], (double)v[1]); 
	}
	inline Vector3d f2d(const Vector3f & v) {
		return Vector3d((double)v[0], (double)v[1], (double)v[2]); 
	}


	inline Matrix2f d2f(const Matrix2d & vv) {
		const double * v = vv; return Matrix2f((float)v[0], (float)v[1], (float)v[2], (float)v[3]);
	}
	inline Matrix3f d2f(const Matrix3d & vv) {
		const double * v = vv; return Matrix3f((float)v[0], (float)v[1], (float)v[2], (float)v[3], (float)v[4], (float)v[5], (float)v[6], (float)v[7], (float)v[8]);
	}
	inline Matrix3d f2d(const Matrix3f & vv) {
		const float * v = vv; return Matrix3d((double)v[0], (double)v[1], (double)v[2], (double)v[3], (double)v[4], (double)v[5], (double)v[6], (double)v[7], (double)v[8]);
	}
	inline Matrix2d f2d(const Matrix2f & vv) {
		const float * v = vv; return Matrix2d((double)v[0], (double)v[1], (double)v[2], (double)v[3]);
	}


	inline AxisAlignedBox2f d2f(const AxisAlignedBox2d & v) {
		return AxisAlignedBox2f((float)v.Min[0], (float)v.Max[0], (float)v.Min[1], (float)v.Max[1] ); 
	}
	inline AxisAlignedBox3f d2f(const AxisAlignedBox3d & v) {
		return AxisAlignedBox3f((float)v.Min[0], (float)v.Max[0], (float)v.Min[1], (float)v.Max[1], (float)v.Min[2], (float)v.Max[2] ); 
	}
	inline AxisAlignedBox2d f2d(const AxisAlignedBox2f & v) {
		return AxisAlignedBox2d((double)v.Min[0], (double)v.Max[0], (double)v.Min[1], (double)v.Max[1]); 
	}
	inline AxisAlignedBox3d f2d(const AxisAlignedBox3f & v) {
		return AxisAlignedBox3d((double)v.Min[0], (double)v.Max[0], (double)v.Min[1], (double)v.Max[1], (double)v.Min[2], (double)v.Max[2]);
	}


	template<class Real>
	inline Real Clamp(const Real & fValue, const Real & fMin, const Real & fMax) {
		if (fValue < fMin)
			return fMin;
		else if (fValue > fMax)
			return fMax;
		else
			return fValue;
	}


	inline void array3f_add( float * pBuffer, unsigned int nIndex, const float * pAdd ) {
		pBuffer[3*nIndex] += pAdd[0]; pBuffer[3*nIndex+1] += pAdd[1]; pBuffer[3*nIndex+2] += pAdd[2];
	}
	inline void array3f_normalize( float * pBuffer, unsigned int nIndex, float fEpsilon = 0.0f ) {
		auto v = Vector3f(&pBuffer[3*nIndex]).Normalized(fEpsilon);
		pBuffer[3*nIndex] = v[0]; pBuffer[3*nIndex+1] = v[1]; pBuffer[3*nIndex+2] = v[2];
	}
	inline void vectorf_push( std::vector<float> & v, const g3::Vector3f & p ) {
		v.push_back(p[0]); v.push_back(p[1]); v.push_back(p[2]);
	}
	inline void vectori_push( std::vector<unsigned int> & v, const g3::Vector2i & p ) {
		v.push_back(p[0]); v.push_back(p[1]);
	}
	inline void vectori_push( std::vector<unsigned int> & v, const g3::Vector3i & p ) {
		v.push_back(p[0]); v.push_back(p[1]); v.push_back(p[2]);
	}


}  // namespace g3


