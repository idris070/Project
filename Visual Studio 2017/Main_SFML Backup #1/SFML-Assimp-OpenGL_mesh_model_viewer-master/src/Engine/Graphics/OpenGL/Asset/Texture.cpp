#include "Texture.hpp"
#include "TextureLoader.hpp"


namespace KG
{
	KG::TextureLoader_SmartPtr	Texture::s_spTextureLoader(nullptr);

	TextureLoader & Texture::GetLoader(void)
	{
		if (s_spTextureLoader == nullptr)
			s_spTextureLoader = KG::TextureLoader_SmartPtr(new KG::TextureLoader());
		check_for_DevIL_error();
		return *s_spTextureLoader;
	}

	Texture::Texture(const DType p_DType, const std::string & p_Path)
		: m_Type(DType::Tex2D), m_GLTextureHandle(0), m_DevILHandle(0)
		, m_FilePath(p_Path)
		, m_Valid(true)
		, m_TexCoordMultiplier(1.0f)
	{
		if (p_Path.length() == 0)
			KE::Debug::print(KE::Debug::DBG_ERROR, "Texture : ctor given empty string.");
		m_FilePath = p_Path;

		KG::Texture::GetLoader();
		ilGenImages(1, &m_DevILHandle);
		ilBindImage(m_DevILHandle);
		if (!ilLoadImage(p_Path.c_str())) // load and if fails
		{
			KE::Debug::print(KE::Debug::DBG_ERROR, "Texture : DevIL ilLoadImage(string) loading failed.");
			KE::Debug::print(KE::Debug::DBG_ERROR, " - > File path = " + p_Path);
			KG::check_for_DevIL_error();
			m_Valid = false;
			return;
		}
		if (!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
		{
			KE::Debug::print(KE::Debug::DBG_ERROR, "Texture : DevIL ilConvertImage(,) failed.");
			m_Valid = false;
			return;
		}

		glGenTextures(1, &m_GLTextureHandle);
		m_TextureTarget = TranslateTargetType(p_DType);
		glBindTexture(m_TextureTarget, m_GLTextureHandle);
		glTexParameteri(m_TextureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // TODO : provide option to change this.
		glTexParameteri(m_TextureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D
		(
			m_TextureTarget								// Texture target.
			, 0										
			, GL_SRGB_ALPHA //ilGetInteger(IL_IMAGE_FORMAT)  # Use GL_SRGB_ALPHA to accomdate Gamma correction in shader. This converts image to linear space?
			, ilGetInteger(IL_IMAGE_WIDTH)
			, ilGetInteger(IL_IMAGE_HEIGHT)
			, 0
			, GL_RGBA //ilGetInteger(IL_IMAGE_FORMAT)
			, ilGetInteger(IL_IMAGE_TYPE)
			, ilGetData()
		);
		glBindTexture(m_TextureTarget, 0);

		KE::Debug::check_for_GL_error();
		ilBindImage(0);
		ilDeleteImage(m_DevILHandle);
	}
	Texture::~Texture(void)
	{
		glDeleteTextures(1, &m_GLTextureHandle);
	}

	void Texture::Bind(GLenum p_GLTextureUnit)
	{
		glActiveTexture(p_GLTextureUnit);
		glBindTexture(m_TextureTarget, m_GLTextureHandle);
	}

	const bool Texture::IsValid(void) const
	{
		return m_Valid;
	}

	const KG::Texture::DType Texture::GetType(void) const
	{
		return m_Type;
	}

	const GLuint Texture::GetGLHandle(void) const
	{
		return m_GLTextureHandle;
	}

	void Texture::SetType(const KG::Texture::DType p_Type)
	{
		m_Type = p_Type;
	}

	void Texture::SetFilePath(const std::string & p_Path)
	{
		m_FilePath = p_Path;
	}

	void Texture::SetGLHandle(const GLuint p_Handle)
	{
		m_GLTextureHandle = p_Handle;
	}

	void Texture::SetDevILHandle(const ILuint p_Handle)
	{
		m_DevILHandle = p_Handle;
	}

	const GLenum Texture::TranslateTargetType(const DType p_DimensionType)
	{
		switch (p_DimensionType)
		{
		case KG::Texture::DType::Tex2D:
			return GL_TEXTURE_2D;
		default:
			KE::Debug::print(KE::Debug::DBG_ERROR, "Texture : unsupported Texture type.");
			return GL_INVALID_ENUM;
		}
	}

}