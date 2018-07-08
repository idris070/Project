#include "Transform.hpp"

namespace KG
{
	const glm::dvec3 Transform::UnitUpVec3(0.0, 1.0, 0.0);
	const glm::dvec3 Transform::UnitForwardVec3(0.0, 0.0, -1.0); // OpenGL +ve z is coming out of the screen.
	const glm::dvec3 Transform::UnitRightVec3(1.0, 0.0, 0.0);

	Transform::Transform(void)
		: m_Scale(1.0, 1.0, 1.0)
		, m_Position(0.0, 0.0, 0.0)
		, m_FinalTransformMatrix(1.0)
		, m_Evaluated(true)
	{}

	Transform::Transform(const glm::mat4 p_Mat4f)
		: m_Scale(glm::length(p_Mat4f[0]), glm::length(p_Mat4f[1]), glm::length(p_Mat4f[2]))
		, m_Position(p_Mat4f[3])
		, m_OrientationQuat(glm::toQuat(glm::dmat4(p_Mat4f)))
		, m_FinalTransformMatrix(p_Mat4f)
		, m_Evaluated(true)
	{}

	Transform::Transform(const glm::dmat4 p_Mat4d)
		: m_Scale(glm::length(p_Mat4d[0]), glm::length(p_Mat4d[1]), glm::length(p_Mat4d[2]))
		, m_Position(p_Mat4d[3])
		, m_OrientationQuat(glm::toQuat(p_Mat4d))
		, m_FinalTransformMatrix(p_Mat4d)
		, m_Evaluated(true)
	{}

	Transform & Transform::SetPosition(const double p_PosX, const double p_PosY, const double p_PosZ)
	{
		m_Position.x = p_PosX; m_Position.y = p_PosY; m_Position.z = p_PosZ;
		m_Evaluated = false; return *this;
	}

	Transform & Transform::SetScale(const double p_ScaleX, const double p_ScaleY, const double p_ScaleZ)
	{
		m_Scale.x = p_ScaleX; m_Scale.y = p_ScaleY; m_Scale.z = p_ScaleZ;
		m_Evaluated = false; return *this;
	}

	Transform & Transform::SetOrientation(const double p_AngleX, const double p_AngleY, const double p_AngleZ)
	{
		m_OrientationQuat = glm::dquat() * glm::angleAxis(p_AngleY, UnitUpVec3);
		m_OrientationQuat = m_OrientationQuat * glm::angleAxis(p_AngleZ, UnitForwardVec3);
		m_OrientationQuat = m_OrientationQuat * glm::angleAxis(p_AngleX, UnitRightVec3);
		m_OrientationQuat = glm::normalize(m_OrientationQuat);
		m_Evaluated = false; return *this;
	}

	Transform & Transform::SetOrientationQuat(const glm::dquat & p_rquat)
	{
		m_OrientationQuat = p_rquat;
		m_Evaluated = false;
		return *this;
	}

	Transform & Transform::SetOrientation(const glm::dmat3 & m_rMat)
	{
		m_OrientationQuat = std::move(glm::quat_cast(m_rMat));
		m_Evaluated = false;
		return *this;
	}

	Transform & Transform::SetPitch(const double p_Angle)
	{ // untested
		m_OrientationQuat.x = 0.0;
		m_OrientationQuat = glm::normalize(m_OrientationQuat);
		return this->OffsetPitch(p_Angle);
	}

	Transform & Transform::SetYaw(const double p_Angle)
	{ // untested
		m_OrientationQuat.y = 0.0;
		m_OrientationQuat = glm::normalize(m_OrientationQuat);
		return this->OffsetYaw(p_Angle);
	}

	Transform & Transform::SetRoll(const double p_Angle)
	{ // untested
		m_OrientationQuat.z = 0.0;
		m_OrientationQuat = glm::normalize(m_OrientationQuat);
		return this->OffsetRoll(p_Angle);
	}

	Transform & Transform::SetOrientationToPosition(const double p_PosX, const double p_PosY, const double p_PosZ)
	{
		return this->SetOrientationToPosition(glm::dvec3(p_PosX, p_PosY, p_PosZ));
	}

	Transform & Transform::SetOrientationToPosition(const glm::dvec3 & p_Pos)
	{// untested
		const glm::dvec3 target(p_Pos);
		if (target == this->GetPositionVec3()) // both in same position
			return *this;
		const glm::dvec3 direction = glm::normalize(target - this->GetPositionVec3());
		const double dot = glm::dot(glm::dvec3(0.0, 0.0, 1.0), direction); // the 1.0 should probably be -1.0
		if (std::abs(dot - (-1.0)) < 0.00001)
		{
			m_OrientationQuat = glm::angleAxis(180.0, glm::dvec3(0.0, 1.0, 0.0));
			m_Evaluated = false; return *this;
		}
		else if (std::abs(dot - (1.0)) < 0.00001)
		{
			m_OrientationQuat = glm::dquat();
			m_Evaluated = false; return *this;
		}
		double angle = -1.0 * KE::Math::RadianToDegree(std::acos(dot));
		const glm::dvec3 cross(glm::normalize(glm::cross(direction, glm::dvec3(0.0, 0.0, 1.0))));
		m_OrientationQuat = glm::angleAxis(angle, cross);
		m_Evaluated = false; return *this;
	}

	Transform & Transform::OffsetScale(const double p_DeltaX, const double p_DeltaY, const double p_DeltaZ)
	{
		m_Scale.x += p_DeltaX; m_Scale.y += p_DeltaY; m_Scale.z += p_DeltaZ;
		m_Evaluated = false; return *this;
	}

	Transform & Transform::OffsetPosition(const double p_DeltaX, const double p_DeltaY, const double p_DeltaZ)
	{
		m_Position.x += p_DeltaX; m_Position.y += p_DeltaY;	m_Position.z += p_DeltaZ;
		m_Evaluated = false; return *this;
	}

	Transform & Transform::OffsetOrientation(const double p_DeltaX, const double p_DeltaY, const double p_DeltaZ)
	{
		this->OffsetPitch(p_DeltaX).OffsetYaw(p_DeltaY).OffsetRoll(p_DeltaZ);
		m_Evaluated = false; return *this;
	}

	Transform & Transform::OffsetPitch(const double p_Angle)
	{
		if (p_Angle == 0.0) return *this;
		m_OrientationQuat = m_OrientationQuat * glm::angleAxis(p_Angle, UnitRightVec3);
		m_Evaluated = false; return *this;
	}

	Transform & Transform::OffsetYaw(const double p_Angle)
	{
		if (p_Angle == 0.0) return *this;
		m_OrientationQuat = m_OrientationQuat * glm::angleAxis(p_Angle, UnitUpVec3);
		m_Evaluated = false;return *this;
	}

	Transform & Transform::OffsetRoll(const double p_Angle)
	{
		if (p_Angle == 0.0) return *this;
		m_OrientationQuat = m_OrientationQuat * glm::angleAxis(p_Angle, UnitForwardVec3);
		m_Evaluated = false;return *this;
	}

	void Transform::StrafeRelativeTo(const double p_DeltaX, const double p_DeltaY, const double p_DeltaZ, KG::Transform & p_rTransform)
	{
		this->StrafeRelativeTo(p_DeltaX, p_DeltaY, p_DeltaZ, p_rTransform.GetOrientationMat());
	}

	void Transform::StrafeRelativeTo(const double p_DeltaX, const double p_DeltaY, const double p_DeltaZ, const glm::dmat4 & p_rOrientation)
	{
		using namespace glm;
		const dvec4 result = p_rOrientation * dvec4(p_DeltaX, p_DeltaY, p_DeltaZ, 1.0);
		m_Position.x += result.x;	m_Position.y += result.y;	m_Position.z +=	result.z;
		m_Evaluated = false;
	}

	//void TransformMatrix::RotateRelativeTo(const double p_AngleX, const double p_AngleY, const double p_AngleZ, const KG::TransformMatrix & p_rTransform)
	//{

	//}

	void Transform::StrafeUp(const double p_Delta)
	{
		this->StrafeRelativeTo(0.0,p_Delta, 0.0, this->GetOrientationMat());
	}

	void Transform::StrafeDown(const double p_Delta)
	{
		this->StrafeRelativeTo(0.0,-p_Delta, 0.0, this->GetOrientationMat());
	}

	void Transform::StrafeLeft(const double p_Delta)
	{
		this->StrafeRelativeTo(-p_Delta, 0.0, 0.0, this->GetOrientationMat());
	}

	void Transform::StrafeRight(const double p_Delta)
	{		
		this->StrafeRelativeTo(p_Delta, 0.0, 0.0, this->GetOrientationMat());			
	}

	void Transform::StrafeForward(const double p_Delta)
	{
		this->StrafeRelativeTo(0.0, 0.0, -p_Delta, this->GetOrientationMat());
	}

	void Transform::StrafeBackward(const double p_Delta)
	{
		this->StrafeRelativeTo(0.0, 0.0, p_Delta, this->GetOrientationMat());
	}

	const glm::dvec3 & Transform::GetPositionVec3(void) const
	{
		return m_Position;
	}

	const glm::dvec3 Transform::GetForwardVec3(void)
	{ // note: this algorithm is much faster than mutipling a default vector with the orientation quaternion (in DEBUG).
		const glm::dquat & q(m_OrientationQuat);
		return glm::normalize( glm::dvec3
		(
			-2.0 * (q.x * q.z + q.w * q.y), 
			-2.0 * (q.y * q.z - q.w * q.x),
			-1.0 + 2.0 * (q.x * q.x + q.y * q.y))
		);
	}

	const glm::dvec3 Transform::GetUpVec3(void)
	{ // note: this algorithm is much faster than mutipling a default vector with the orientation quaternion (in DEBUG).
		const glm::dquat & q(m_OrientationQuat);
		return glm::normalize( glm::dvec3
		(
			2.0 * (q.x * q.y - q.w * q.z), 
			1.0 - 2.0 * (q.x * q.x + q.z * q.z),
			2.0 * (q.y * q.z + q.w * q.x))
		);
	}

	const glm::dvec3 Transform::GetRightVec3(void)
	{ // note: this algorithm is much faster than mutipling a default vector with the orientation quaternion (in DEBUG).
		const glm::dquat & q(m_OrientationQuat);
		return glm::normalize( glm::dvec3
		(
			1.0 - 2.0 * (q.y * q.y + q.z * q.z),
			2.0 * (q.x * q.y + q.w * q.z),
			2.0 * (q.x * q.z - q.w * q.y))
		);
	}

	const glm::dmat4 Transform::GetPositionMat(void) const
	{
		return glm::translate(m_Position);
	}

	//void TransformMatrix::RotateRelativeTo(const double p_AngleX, const double p_AngleY, const double p_AngleZ, const glm::dvec3 p_Axis)
	//{
	//	glm::dquat rotation = glm::normalize(glm::angleAxis(p_AngleY, p_Axis) * glm::dquat());
	//	rotation = glm::normalize(glm::angleAxis(p_AngleX, p_Axis) * rotation); // X come after Y
	//	rotation = glm::normalize(glm::angleAxis(p_AngleZ, p_Axis) * rotation);
	//	m_Orientation = rotation * m_Orientation;
	//	m_Angles = glm::eulerAngles(m_Orientation) + m_Angles;
	//}

	const glm::dquat Transform::GetOrientationQuat()
	{
		return m_OrientationQuat = glm::normalize(m_OrientationQuat);
	}

	const glm::dvec3 Transform::GetEulerAngles(void) const
	{
		return glm::eulerAngles(m_OrientationQuat);
	}

	const glm::dmat4 Transform::GetOrientationMat(void)
	{
		return glm::mat4_cast(m_OrientationQuat = glm::normalize(m_OrientationQuat));
	}

	const GLfloat * const Transform::GetRawPtrF(void)
	{
		this->Evaluate();
		return glm::value_ptr(glm::fmat4(m_FinalTransformMatrix));
	}

	const glm::dmat4 & Transform::GetGLMMatd(void)
	{
		this->Evaluate();
		return m_FinalTransformMatrix;
	}

	const glm::mat4 Transform::GetGLMMatf(void)
	{
		return glm::fmat4(this->GetGLMMatd());
	}

	Transform & Transform::Evaluate(void)
	{
		if (!m_Evaluated)
		{
			m_FinalTransformMatrix = glm::translate(m_Position) * this->GetOrientationMat() * glm::scale(m_Scale);
			m_Evaluated = true;
		}
		return *this;
	}

	Transform operator*(Transform & p_rTM, const glm::dmat4 p_GLMMat4)
	{
		return p_rTM.GetGLMMatd() * p_GLMMat4;
	}
	
	Transform operator*(const glm::dmat4 p_GLMMat4, Transform & p_rTM)
	{
		return p_GLMMat4 * p_rTM.GetGLMMatd();
	}

	Transform operator*(Transform & p_TM,const glm::mat4 & p_rGLMMat4)
	{
		return p_TM * glm::dmat4(p_rGLMMat4);
	}

	Transform operator*(const glm::mat4 p_GLMMat4, Transform & p_rTM)
	{
		return glm::dmat4(p_GLMMat4) * p_rTM.GetGLMMatd();
	}

	Transform operator*(Transform & p_rTMLeft, Transform & p_rTMRight)
	{
		return p_rTMLeft.GetGLMMatd() * p_rTMRight.GetGLMMatd();
	}

	const glm::dmat4 mat4_cast(KG::Transform & p_rTransformMatrix)
	{
		return glm::dmat4(p_rTransformMatrix.GetGLMMatd());
	}

} // KG ns
