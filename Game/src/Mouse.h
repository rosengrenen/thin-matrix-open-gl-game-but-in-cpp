#pragma once

class Mouse
{
private:
	glm::vec2 m_position;
	glm::vec2 m_offset;
public:

	void prepare(GLFWwindow* window)
	{
		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		m_offset.x = m_position.x - mouseX;
		m_offset.y = mouseY - m_position.y;

		m_position.x = mouseX;
		m_position.y = mouseY;
	}

	glm::vec2 getOffset()
	{
		return m_offset;
	}
}; 