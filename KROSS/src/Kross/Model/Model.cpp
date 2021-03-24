#include <Kross_pch.h>
#include "Model.h"

/*
namespace Kross::Model {
    Base::Base(const Mesh& mesh)
    {
        addData(mesh);
    }

    Base::~Base()
    {
        deleteData();
    }

    void Base::bindVAO() const
    {
        glBindVertexArray(m_vao);
    }

    void Base::addData(const Mesh& mesh)
    {
        if (m_vao != 0)
            deleteData();

        m_indicesCount = mesh.indices.size();

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        addVBO(3, mesh.vertexPositions);
        addVBO(2, mesh.textureCoords);
        addEBO(mesh.indices);
    }

    void Base::addVBO(int dimensions, const std::vector<GLfloat>& data)
    {
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER,
            data.size() * sizeof(GLfloat),
            data.data(),
            GL_STATIC_DRAW);

        glVertexAttribPointer(m_vboCount,
            dimensions,
            GL_FLOAT,
            GL_FALSE,
            0,
            (GLvoid*)0);

        glEnableVertexAttribArray(m_vboCount++);

        m_buffers.push_back(vbo);
    }

    void Base::addEBO(const std::vector<GLuint>& indices)
    {
        GLuint ebo;
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            indices.size() * sizeof(GLuint),
            indices.data(),
            GL_STATIC_DRAW);
    }

    void Base::deleteData()
    {
        glDeleteVertexArrays(1, &m_vao);
        glDeleteBuffers(m_buffers.size(),
            m_buffers.data());

        m_buffers.clear();

        m_vboCount = 0;
        m_vao = 0;
        m_indicesCount = 0;
    }

    int Base::getIndicesCount() const
    {
        return m_indicesCount;
    }
}
*/