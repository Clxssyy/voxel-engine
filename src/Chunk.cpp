#include "Chunk.hpp"

Chunk::Chunk(glm::vec3 position) : position(position * glm::vec3(SIZE, HEIGHT, SIZE)) {};

void Chunk::Draw(Shader& shader, Camera& camera) {
  shader.Use();
  vao.Bind();
  camera.Matrix(shader, "camMatrix");
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  vao.Unbind();
};

void Chunk::Generate() {
  for (int x = 0; x < SIZE; x++)
    for (int y = 0; y < HEIGHT; y++)
      for (int z = 0; z < SIZE; z++) {
          if (x == 0 || x == SIZE - 1 || y == 0 || y == HEIGHT - 1 || z == 0 || z == SIZE - 1)
            for (const auto &vertex : voxels[x][y][z].vertices)
              vertices.push_back(vertex);
          if (x == 0)
            for (const auto &index : voxels[x][y][z].faces["left"])
              indices.push_back(index + vertices.size() - 8);
          if (x == SIZE - 1)
            for (const auto &index : voxels[x][y][z].faces["right"])
              indices.push_back(index + vertices.size() - 8);
          if (y == 0)
            for (const auto &index : voxels[x][y][z].faces["bottom"])
              indices.push_back(index + vertices.size() - 8);
          if (y == HEIGHT - 1)
            for (const auto &index : voxels[x][y][z].faces["top"])
              indices.push_back(index + vertices.size() - 8);
          if (z == 0)
            for (const auto &index : voxels[x][y][z].faces["back"])
              indices.push_back(index + vertices.size() - 8);
          if (z == SIZE - 1)
            for (const auto &index : voxels[x][y][z].faces["front"])
              indices.push_back(index + vertices.size() - 8);
          // Render all faces
          // for (const auto &face : voxel.faces)
          //   for (const auto &index : face.second)
          //     indices.push_back(index + vertices.size() - 8);
      }
};

void Chunk::GenerateBlocks() {
    for (int x = 0; x < SIZE; x++)
      for (int z = 0; z < SIZE; z++)
        for (int y = 0; y < HEIGHT; y++) {
          Voxel voxel(glm::vec3(x, y, z) + position, glm::vec3(x, y, z) / glm::vec3(SIZE, HEIGHT, SIZE));
          voxels[x][y][z] = voxel;
      };
}

void Chunk::GenerateGreedy() {
  Vertex FBL = voxels[0][0][0].vertices[0];
  Vertex FBR = voxels[0][0][0].vertices[1];
  Vertex FTR = voxels[0][0][0].vertices[2];
  Vertex FTL = voxels[0][0][0].vertices[3];
  
  Vertex BBL = voxels[0][0][0].vertices[4];
  Vertex BBR = voxels[0][0][0].vertices[5];
  Vertex BTR = voxels[0][0][0].vertices[6];
  Vertex BTL = voxels[0][0][0].vertices[7];

  bool searched[SIZE][HEIGHT][SIZE] = {false};

  for (int x = 0; x < SIZE; x++) {
    for(int z = 0; z < SIZE; z++) {
      for (int y = 0; y < HEIGHT; y++) {
        // if (searched[x][y][z]) continue;
        searched[x][y][z] = true;

        // Back face
        if (z == 0) {
          if (y == HEIGHT - 1) break;
          if (voxels[x][y][z].vertices[7].Position.y == BTL.Position.y) {
            BTL = voxels[x][y+1][z].vertices[7];
          }
          if (voxels[x][y][z].vertices[6].Position.y == BTR.Position.y) {
            BTR = voxels[x][y+1][z].vertices[6];
          }
        }

        // Front face
        if (z == SIZE - 1) {
          if (y == HEIGHT - 1) break;
          if (voxels[x][y][z].vertices[3].Position.y == FTL.Position.y) {
            FTL = voxels[x][y+1][z].vertices[3];
          }
          if (voxels[x][y][z].vertices[2].Position.y == FTR.Position.y) {
            FTR = voxels[x][y+1][z].vertices[2];
          }
        }

        // if (y == HEIGHT - 1) break;
        // if (voxels[x][y][z].vertices[3].Position.x == FTL.Position.x) {
        //   FTL = voxels[x][y+1][0].vertices[3];
        // }
        // if (voxels[x][y][z].vertices[2].Position.x == FTR.Position.x) {
        //   FTR = voxels[x][y+1][z].vertices[2];
        // }
      }
      // Back face
      if (z == 0) {
        if (x == SIZE - 1) break;
        if (voxels[x][0][z].vertices[5].Position.z == BTL.Position.z) {
          BBR = voxels[x+1][0][z].vertices[5];
        }
        if (voxels[x][0][z].vertices[6].Position.x == BTR.Position.x) {
          BTR = voxels[x+1][0][z].vertices[6];
        }
      }

      // Front face
      if (z == SIZE - 1) {
        if (x == SIZE - 1) break;
        if (voxels[x][0][z].vertices[1].Position.x == FBR.Position.x) {
          FBR = voxels[x+1][0][z].vertices[1];
        }
        if (voxels[x][0][z].vertices[0].Position.x == FBL.Position.x) {
          FBL = voxels[x][0][z].vertices[0];
        }
        if (voxels[x][0][z].vertices[2].Position.z == FTR.Position.z) {
          FTR = voxels[x+1][HEIGHT-1][z].vertices[2];
        }
      }
      // if (x == SIZE - 1) break;
      // if (voxels[x][0][z].vertices[1].Position.z == FBR.Position.z) {
      //   FBR = voxels[x+1][0][z].vertices[1];
      // }
      // if (voxels[x][0][z].vertices[2].Position.z == FTR.Position.z) {
      //   FTR = voxels[x+1][0][z].vertices[2];
      // }
    }
  }

    vertices.push_back(FBL);
    vertices.push_back(FBR);
    vertices.push_back(FTR);
    vertices.push_back(FTL);

    vertices.push_back(BBL);
    vertices.push_back(BBR);
    vertices.push_back(BTR);
    vertices.push_back(BTL);

    // Back face
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);
    
    // Front face
    indices.push_back(5);
    indices.push_back(4);
    indices.push_back(7);
    indices.push_back(5);
    indices.push_back(7);
    indices.push_back(6);

    // Right face
    indices.push_back(1);
    indices.push_back(5);
    indices.push_back(6);
    indices.push_back(1);
    indices.push_back(6);
    indices.push_back(2);

    // Left face
    indices.push_back(4);
    indices.push_back(0);
    indices.push_back(3);
    indices.push_back(4);
    indices.push_back(3);
    indices.push_back(7);

    // Top face
    indices.push_back(3);
    indices.push_back(2);
    indices.push_back(6);
    indices.push_back(3);
    indices.push_back(6);
    indices.push_back(7);

    // Bottom face
    indices.push_back(4);
    indices.push_back(5);
    indices.push_back(1);
    indices.push_back(4);
    indices.push_back(1);
    indices.push_back(0);

    // vertices.push_back(FBL);
    // vertices.push_back(FBR);
    // vertices.push_back(FTR);
    // vertices.push_back(FTL);
    // indices.push_back(0);
    // indices.push_back(1);
    // indices.push_back(2);
    // indices.push_back(0);
    // indices.push_back(2);
    // indices.push_back(3);
};

void Chunk::Greedy(std::string face) {

}

void Chunk::Build() {
  vao.Bind();

  vbo = VBO(vertices);
  ebo = EBO(indices);

  vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
  vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

  vao.Unbind();
  vbo.Unbind();
  ebo.Unbind();
};

void Chunk::Delete() {
  vao.Delete();
  vbo.Delete();
  ebo.Delete();
};
