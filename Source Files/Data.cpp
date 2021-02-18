#include "Data.h"


void diamerizo(GLfloat* data, int length, GLfloat* final_data) {

	int j = 0;

	for (int i = 0; i < length; i += 9) {
		GLfloat x1 = data[i + 0]; GLfloat y1 = data[i + 1]; GLfloat z1 = data[i + 2];
		GLfloat x2 = data[i + 3]; GLfloat y2 = data[i + 4]; GLfloat z2 = data[i + 5];
		GLfloat x3 = data[i + 6]; GLfloat y3 = data[i + 7]; GLfloat z3 = data[i + 8];

		final_data[j + 0] = x1;            final_data[j + 1] = y1;            final_data[j + 2] = z1;
		final_data[j + 3] = (x1 + x2) / 2; final_data[j + 4] = (y1 + y2) / 2; final_data[j + 5] = (z1 + z2) / 2;
		final_data[j + 6] = (x1 + x3) / 2; final_data[j + 7] = (y1 + y3) / 2; final_data[j + 8] = (z1 + z3) / 2;

		final_data[j + 9] = (x1 + x2) / 2; final_data[j + 10] = (y1 + y2) / 2; final_data[j + 11] = (z1 + z2) / 2;
		final_data[j + 12] = (x1 + x3) / 2; final_data[j + 13] = (y1 + y3) / 2; final_data[j + 14] = (z1 + z3) / 2;
		final_data[j + 15] = (x2 + x3) / 2; final_data[j + 16] = (y2 + y3) / 2; final_data[j + 17] = (z2 + z3) / 2;

		final_data[j + 18] = (x1 + x2) / 2; final_data[j + 19] = (y1 + y2) / 2; final_data[j + 20] = (z1 + z2) / 2;
		final_data[j + 21] = x2;            final_data[j + 22] = y2;            final_data[j + 23] = z2;
		final_data[j + 24] = (x2 + x3) / 2; final_data[j + 25] = (y2 + y3) / 2; final_data[j + 26] = (z2 + z3) / 2;

		final_data[j + 27] = (x1 + x3) / 2; final_data[j + 28] = (y1 + y3) / 2; final_data[j + 29] = (z1 + z3) / 2;
		final_data[j + 30] = (x2 + x3) / 2; final_data[j + 31] = (y2 + y3) / 2; final_data[j + 32] = (z2 + z3) / 2;
		final_data[j + 33] = x3;            final_data[j + 34] = y3;            final_data[j + 35] = z3;

		j += 36;
	}
}

void normalizeSphere(GLfloat* data, int length, GLfloat* final_data, int radius) {

	for (int i = 0; i < length; i += 3) {

		glm::vec3 vector(data[i + 0], data[i + 1], data[i + 2]);
		vector = glm::normalize(vector);

		final_data[i + 0] = vector.x * radius;     final_data[i + 1] = vector.y * radius;      final_data[i + 2] = vector.z * radius;
	}
}

void UVcreateSphere(GLfloat* data, int length, GLfloat* final_data) {

	int j = 0;
	
	for (int i = 0; i < length; i += 3) {
		glm::vec3 vector(data[i + 0], data[i + 1], data[i + 2]);

		GLfloat U = 0.5 + atan2(vector.x, vector.z) / (2 * PI);
		GLfloat V = 0.5 - asin(vector.y) / PI ;

		final_data[j + 0] = U; final_data[j + 1] = V;

		j += 2;
	}
}

void NormalsCreateSphere(GLfloat* data, int length, GLfloat* final_data) {

	std::vector<glm::vec3> TriangleNormals;
	int k = 0;

	for (int i = 0; i < length; i += 9) {

		glm::vec3 vectorA(data[i + 0], data[i + 1], data[i + 2]);
		glm::vec3 vectorB(data[i + 3], data[i + 4], data[i + 5]);
		glm::vec3 vectorC(data[i + 6], data[i + 7], data[i + 8]);

		glm::vec3 vectorTriangle = glm::normalize(glm::cross(vectorB - vectorA, vectorC - vectorA));

		TriangleNormals.push_back(vectorTriangle);

	}

	for (int i = 0; i < length; i += 3) {

		glm::vec3 vector1(data[i + 0], data[i + 1], data[i + 2]);
		std::vector<int> triangles;

		// Find adjacent triangles to vector1
		for (int j = 0; j < length; j += 9) {

			glm::vec3 vectorA(data[j + 0], data[j + 1], data[j + 2]);
			glm::vec3 vectorB(data[j + 3], data[j + 4], data[j + 5]);
			glm::vec3 vectorC(data[j + 6], data[j + 7], data[j + 8]);
			
			if (vector1 == vectorA || vector1 == vectorB || vector1 == vectorC) {
				triangles.push_back(j / 9);
			} 
		}

		glm::vec3 sum (0, 0, 0);

		for (int j = 0; j < triangles.size(); j++) {
			
			sum += TriangleNormals[triangles[j]];
		}
		
		if (sum.x != 0) sum.x = sum.x / abs(sum.x);
		if (sum.y != 0) sum.y = sum.y / abs(sum.y);
		if (sum.z != 0) sum.z = sum.z / abs(sum.z);

		//std::cout << sum.x << " " << sum.y << " " << sum.z << std::endl;

		final_data[k] = sum.x; final_data[k + 1] = sum.y; final_data[k + 2] = sum.z;
		k += 3;
	}
}

void NormalsCreateCube(GLfloat* data, int length, GLfloat* final_data) {

	for (int i = 0; i < length; i += 9) {

		glm::vec3 vectorA(data[i + 0], data[i + 1], data[i + 2]);
		glm::vec3 vectorB(data[i + 3], data[i + 4], data[i + 5]);
		glm::vec3 vectorC(data[i + 6], data[i + 7], data[i + 8]);

		glm::vec3 vectorTriangle = glm::normalize(glm::cross(vectorB - vectorA, vectorC - vectorA));

		final_data[i + 0] = vectorTriangle.x; final_data[i + 1] = vectorTriangle.y; final_data[i + 2] = vectorTriangle.z;
		final_data[i + 3] = vectorTriangle.x; final_data[i + 4] = vectorTriangle.y; final_data[i + 5] = vectorTriangle.z;
		final_data[i + 6] = vectorTriangle.x; final_data[i + 7] = vectorTriangle.y; final_data[i + 8] = vectorTriangle.z;

	}

}

void NormalsCreate(GLfloat* data, int length, GLfloat* final_data, glm::vec3 center) {

	glm::vec3 Vector;

	for (int i = 0; i < length; i += 3) {

		float x = data[i]; float y = data[i + 1]; float z = data[i + 2];

		Vector.x = center.x - x;	Vector.y = center.y - y;	Vector.z = center.z - z;

		Vector = normalize(Vector);

		final_data[i] = -Vector.x;		final_data[i + 1] = -Vector.y;		final_data[i + 2] = -Vector.z;
	}
}


