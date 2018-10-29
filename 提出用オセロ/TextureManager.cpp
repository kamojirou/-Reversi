#include "TextureManager.h"
#include"Graphics.h"
#include"Texture2D.h"
#include<assert.h>

// Ã“Iƒƒ“ƒo•Ï”‚ÌŽÀ‘Ì‚ðéŒ¾
TextureManager * TextureManager::s_instance = nullptr;


TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
}

void TextureManager::Startup()
{
	assert(s_instance == nullptr);
	s_instance = new TextureManager();
}

void TextureManager::Shutdown()
{
	assert(s_instance != nullptr);
	delete s_instance;
	s_instance = nullptr;

}

Texture2D * TextureManager::RegisterTexture(const char * path, int textureID)
{
	Graphics * g = Graphics::GetInstance();
	Texture2D * texture2D = g->LoadTexture(path);
	m_textures.insert({ textureID,texture2D });
	return texture2D;
}


Texture2D * TextureManager::FindTexture(int textureID)
{
	auto it = m_textures.find(textureID);
	if (it == m_textures.end())
	{
		// Œ©‚Â‚©‚ç‚È‚©‚Á‚½
		return nullptr;
	}

	// Œ©‚Â‚©‚Á‚½
	return (*it).second;
}


void TextureManager::UnregisterTexture(int textureID)
{
	auto it = m_textures.find(textureID);
	delete (*it).second;
	m_textures.erase(it);
}


void TextureManager::Clear()
{
	for (auto it = m_textures.begin(); it != m_textures.end(); it++)
	{
		delete (*it).second;
	}

	m_textures.clear();
}
