#include "Label.h"

namespace Engine {namespace Graphics
{
	Label::Label(std::string text, Maths::vec3 position, Maths::vec4 color): Renderable2D(), m_Text(text)
	{
		m_Position = position;
		m_Color = color;
	}
	void Label::submit(Renderer2D* renderer) const
	{
		renderer->drawString(m_Text, m_Position, m_Color);
	}
}}