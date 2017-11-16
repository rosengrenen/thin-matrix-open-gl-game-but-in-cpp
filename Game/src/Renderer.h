#pragma once

class Renderer
{
public:
	void prepare()
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glEnable(GL_DEPTH_TEST);
	}

	void stop()
	{
		glDisable(GL_DEPTH_TEST);
	}
};