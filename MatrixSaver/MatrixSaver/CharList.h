#pragma once
#include <list>
#include <glm/glm.hpp>

struct Symbol {//�����ַ�
	glm::vec4 Pos;//λ�á���С
	glm::vec4 Color;//��ɫ
	glm::vec4 Coord;//����������ƶ�
};

#define M_PI 3.1415926535

class CharList{
public:
	CharList(float zn, float zf, float ap, float fy);

	~CharList() = default;

	void Move(float dt);//�ƶ�

	bool IsOufScreen();//�ж��Ƿ�Ʈ����Ļ��
	
	std::list<Symbol> necklace;

	glm::vec3 pos;//��βλ��
	glm::vec3 vel;//�ٶȷ���

	int num;//�ַ�����
	
private:
	float size, znear, zfar, aspect, fovy;
	
	void RndPos();//���λ��

	inline float Rand0_1() {//0-1�������
		return (double)rand() / (double)RAND_MAX;
	}
};

