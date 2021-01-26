#ifndef _CUBE_HPP_
#define _CUBE_HPP_

struct Coord3D {
	float x, y, z;

	inline void set(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

class Transform {
public:
	Transform();

	void setIdentity();

	void setTranslation(float x, float y, float z);
	void translate(float x, float y, float z);

	void setRotation(float yaw, float pitch, float roll);
	void rotate(float yaw, float pitch, float roll);

	inline const float* getMatrix() const { return mat; }

	void print();

private:
	float mat[16];
};

class Cube {
public:

	static const int CUBE_SIZE = 0.5f;

	void draw();

	inline Coord3D& getPosition() {
		return position;
	}

	inline Transform& getTransform() {
		return transform;
	}

private:
	Coord3D position;
	Transform transform;
};

#endif