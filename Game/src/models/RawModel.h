#ifndef RAW_MODEL_H
#define RAW_MODEL_H

class RawModel
{
private:
	int m_vaoID;
	int m_vertexCount;
public:
	RawModel(const int vaoID, const int vertexCount) :
		m_vaoID(vaoID), m_vertexCount(vertexCount)
	{ }

	int getVaoID() const
	{
		return m_vaoID;
	}

	int getVertexCount() const
	{
		return m_vertexCount;
	}
};

#endif