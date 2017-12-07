#pragma once

#include "renderable2d.h"

namespace Engine{namespace Graphics
{
	class Label : public Renderable2D
	{
	public:
		Label(std::string text, Maths::vec3 position, Maths::vec4 color);
		void submit(Renderer2D* renderer)const override;
	private: 
		std::string m_Text;
		Maths::vec3 m_Position;
		Maths::vec4 m_Color;
	};
}}
