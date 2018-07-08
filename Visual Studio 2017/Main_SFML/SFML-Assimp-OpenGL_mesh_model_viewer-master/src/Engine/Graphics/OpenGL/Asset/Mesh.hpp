#pragma once
#include "../../../common.hpp"
#include "../Maths.hpp"
#include "../SceneNode.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "../Light/Material.hpp"

namespace KG
{	
	class Texture;
	typedef std::shared_ptr<KG::Texture>	Texture_SmartPtr;
	typedef std::weak_ptr<KG::Texture>		Texture_WeakPtr;
	class Skeleton;
	typedef std::shared_ptr<KG::Skeleton>	Skeleton_SmartPtr;
	typedef std::weak_ptr<KG::Skeleton>		Skeleton_WeakPtr;

	enum class PrimitiveType
	{
		Invalid,
		Points,
		Lines,
		LineStrip,
		Triangles,
		TriangleStrip,
		TriangleFan,
	};


	/*! \class Mesh

		- A Mesh could contain other Mesh's as children in a tree like structure.

	*/
	class Mesh
		: public KG::SceneNode
	{
		friend class MeshLoader;
		friend class SceneLoader;

	public:
		enum class RenderMode
		{
			Null,
			Arrays,
			Indexed
		};

	protected:
		KG::Vec3fVector			m_PosVertices;
		KG::Vec1uVector			m_Indices;
		KG::Vec3fVector			m_NormalVertices;
		KG::Vec3fVector			m_TexCoordVertices;
		KG::Vec4fVector			m_ColorVertices;
		KG::Material			m_Material;
		KG::Skeleton_SmartPtr	m_spSkeleton;
		/* mesh properties. */
		bool					m_LoadedToGPU;		// loaded into GPU memory.
		bool					m_Loaded;			// loaded into RAM.
		/* render specifics. VAO and VBO's */
		GLuint					m_VAO;
		GLuint					m_VertexVBO;		// vertex data buffer
		GLuint					m_IndexVBO;			// index data buffer
		GLuint					m_NormalVBO;
		GLuint					m_TexCoordVBO;
		GLuint					m_ColorVBO;
		GLuint					m_BoneIDVBO;
		GLuint					m_BoneWeightVBO;
		/* texture related. */
		KG::Texture_SmartPtr	m_spTexture;
		/* light related. */
		bool					m_LightBackFace;	// light with only ambient if false.
		/* OpenGL draw call related. */
		RenderMode				m_RenderMode;			// glDrawArrays or glDrawElements
		KG::PrimitiveType		m_PrimitiveType;
		GLenum					m_IndexVarType;			// for glDrawElements
		GLuint					m_FirstIndex;
		GLuint					m_ElementCount;			// number of 'indices' to draw
		GLuint					m_IndexOffset;			// offset in bytes
		// e.g. glDrawArrays(m_PrimitiveType, m_FirstIndex, m_IndexCount)
		// e.g. glDrawElements(m_PrimitiveType, m_ElementCount, m_IndexVarType, m_IndexOffset)

	public:
		Mesh(const KE::EntityID p_EntityID = KE::EntityIDGenerator::NewID(), const KG::RenderPass p_RenderPass = KG::RenderPass::Dynamic);
		virtual ~Mesh(void);

		const KG::PrimitiveType GetPrimitiveType(void) const;
		const GLenum GetGLPrimitiveType(void) const;
		const GLuint GetVAO(void) const;
		const GLuint GetVertexVBO(void) const;
		const GLuint GetIndexVBO(void) const;
		const KG::Vec3fVector & GetVertices(void) const;
		const std::vector<GLuint> & GetIndices(void) const;
		const KG::Vec3fVector & GetTexCoords(void) const;
		const KG::Vec4fVector & GetColors(void) const;
		const KG::Vec3fVector & GetNormals(void) const;
		const Mesh::RenderMode GetRenderMode(void) const;
		const GLenum GetIndexVarType(void) const;
		const GLuint GetFirstIndex(void) const;
		/*! Return the number of primitives. */
		const GLuint GetNumIndex(void) const;
		/*! Return the number of indices there if the Mesh uses IndexedDraw. */
		const GLuint GetNumElement(void) const;
		const GLuint GetIndexOffset(void) const;
		const KG::Material & GetMaterial(void) const;
		const KG::Texture_SmartPtr GetTexture(void) const;
		const KG::Skeleton_SmartPtr GetSkeleton(void) const;

		const bool HasVertices(void) const;
		const bool HasVertexIndices(void) const;
		const bool HasNormals(void) const;
		const bool HasVertexColors(void) const;
		const bool HasTextureCoords(void) const;
		const bool HasTexture(void) const;
		const bool HasMaterial(void) const;
		const bool HasSkeleton(void) const;

		const bool IsBackFaceLit(void) const;

		void SetMaterial(const KG::Material p_NewMaterial);

		/*! Check and see if this Mesh is empty. */
		const bool Loaded(void) const;
		/*! Buffer everything to GPU and set ID's for all VAO and VBO's. */
		virtual const bool BufferAll(void);
		/*! Check to see if mesh is already buffered to the GPU memory. */
		const bool Buffered(void) const;

		virtual void Update(KG::Scene & p_rScene, const KE::Duration p_Elapsed) override;

	//protected:
		void SetVAO(const GLuint p_VAO);
		void SetPosVBO(const GLuint p_VBO);
		void SetIndexVBO(const GLuint p_VBO);
		void SetNormalVBO(const GLuint p_VBO);
		void SetTexCoordVBO(const GLuint p_VBO);
		void SetColorVBO(const GLuint p_VBO);
		void SetBoneIDVBO(const GLuint p_VBO);
		void SetRenderMode(const RenderMode p_RenderMode);
		void SetPrimitiveType(const KG::PrimitiveType p_PrimitiveType);
		void SetIndexType(const GLenum p_IndexType);
		void SetFirstIndex(const GLuint p_FirstIndex);
		void SetIndexOffset(const GLuint p_IndexOffset);
		void SetLightBackFace(const bool p_LightBackFace);
		virtual void SetTexture(KG::Texture_SmartPtr p_spTexture);
		void SetSkeleton(KG::Skeleton_SmartPtr p_spSkeleton);
	};

	typedef std::shared_ptr<KG::Mesh> Mesh_SmartPtr;
	typedef std::weak_ptr<KG::Mesh> Mesh_WeakPtr;
	typedef std::vector<KG::Mesh_SmartPtr>	Mesh_SP_List;
} // KG ns
