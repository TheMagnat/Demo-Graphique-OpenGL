#pragma once

#include <list>
#include <vector>
#include <string>
#include <glm/glm.hpp>

#include "head.hpp"

//Fonction pour charger la texture
#include "3Dfunction.hpp"


/**
*	Classe pour gerer un système de particules.
*	Cette classe est beaucoup inspiré du cours sur les particules de http://www.opengl-tutorial.org
**/
class Particles {
public:
	struct Particle{

		Particle(glm::vec3 const& pos, glm::vec3 const& speed, glm::vec4 const& color, float size, float angle, float weight, float life);

		bool operator<(Particle const& compareWith) const;

		glm::vec3 pos_;
		glm::vec3 speed_;
		glm::vec4 color_;
		float size_, angle_, weight_;
		float life_; // Remaining life of the particle. if < 0 : dead and unused.
		float lifeTime_; //Starting time

		float cameradistance; // *Squared* distance to the camera. if dead : -1.0f
	};


public:

	Particles();
	explicit Particles(std::string const& texture, unsigned int pid, size_t maxSize);

	void init(std::string const& texture, unsigned int pid, size_t maxSize);

	void add(glm::vec3 const& pos, glm::vec3 const& speed, glm::vec4 const& color, float size, float angle, float weight, float life);

	void update(float deltaTime, glm::vec3 const& camPos);

	void draw(glm::mat4 const& view, glm::mat4 const& projection);

private:

	size_t maxSize_;

	unsigned int vao_;

	std::vector<float> quadData_;
	unsigned int dataVbo_;

	std::vector<float> posData_;
	unsigned int posVbo_;

	std::vector<float> colorData_;
	unsigned int colorVbo_;

	std::vector<float> timeData_;
	unsigned int timeVbo_;


	std::list<Particle> allParticles_;

	//Id des shaders pour draw les particules
	unsigned int pid_;
	//Id de la texture
	unsigned int textureId_;

	
};