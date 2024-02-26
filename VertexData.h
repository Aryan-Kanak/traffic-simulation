#ifndef VERTEX_DATA_H
#define VERTEX_DATA_H

struct VertexData {
	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;

	VertexData();
	VertexData(float *vertices, unsigned long long verticesSize, unsigned int *indices, unsigned long long indicesSize);
};

#endif // !VERTEX_DATA_H
