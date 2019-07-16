#include "Particles.hpp"
#include <iostream>
#include <algorithm>


//Particle
Particles::Particle::Particle(glm::vec3 const& pos, glm::vec3 const& speed, glm::vec4 const& color, float size, float angle, float weight, float life)
: pos_(pos), speed_(speed), color_(color), size_(size), angle_(angle), weight_(weight), life_(life), lifeTime_(life) {}

bool Particles::Particle::operator<(Particle const& compareWith) const {
	return cameradistance > compareWith.cameradistance;
}

//Particles

Particles::Particles() : quadData_({-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, 0.5f, 0.0f, 0.5f, 0.5f, 0.0f}) {}

Particles::Particles(std::string const& texture, unsigned int pid, size_t maxSize)
: maxSize_(maxSize), quadData_({-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, 0.5f, 0.0f, 0.5f, 0.5f, 0.0f}), pid_(pid), textureId_(loadTextureA(texture))
{

	posData_.resize(maxSize_*4);
	colorData_.resize(maxSize_*4);

	glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);


	glGenBuffers(1, &dataVbo_);
	glBindBuffer(GL_ARRAY_BUFFER, dataVbo_);
	glBufferData(GL_ARRAY_BUFFER, quadData_.size() * sizeof(float), quadData_.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);


	// The VBO containing the positions and sizes of the particles
	glGenBuffers(1, &posVbo_);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo_);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, maxSize_ * 4 * sizeof(float), NULL, GL_STREAM_DRAW);
	glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*) 0);


	// The VBO containing the colors of the particles
	glGenBuffers(1, &colorVbo_);
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo_);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, maxSize_ * 4 * sizeof(float), NULL, GL_STREAM_DRAW);
	glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (void*) 0);

    glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
    glVertexAttribDivisor(1, 1); // positions : one per quad (its center) -> 1
    glVertexAttribDivisor(2, 1); // color : one per quad -> 1

    glBindVertexArray(0);
}

void Particles::init(std::string const& texture, unsigned int pid, size_t maxSize){

	maxSize_ = maxSize;
	pid_ = pid;
	textureId_ = loadTextureA(texture);

	posData_.resize(maxSize_*4);
	colorData_.resize(maxSize_*4);

	glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);


	glGenBuffers(1, &dataVbo_);
	glBindBuffer(GL_ARRAY_BUFFER, dataVbo_);
	glBufferData(GL_ARRAY_BUFFER, quadData_.size() * sizeof(float), quadData_.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);


	// The VBO containing the positions and sizes of the particles
	glGenBuffers(1, &posVbo_);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo_);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, maxSize_ * 4 * sizeof(float), NULL, GL_STREAM_DRAW);
	glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*) 0);


	// The VBO containing the colors of the particles
	glGenBuffers(1, &colorVbo_);
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo_);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, maxSize_ * 4 * sizeof(float), NULL, GL_STREAM_DRAW);
	glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (void*) 0);

    // The VBO the remaining time
	glGenBuffers(1, &timeVbo_);
	glBindBuffer(GL_ARRAY_BUFFER, timeVbo_);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, maxSize_ * 1 * sizeof(float), NULL, GL_STREAM_DRAW);
	glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, (void*) 0);

    glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
    glVertexAttribDivisor(1, 1); // positions : one per quad (its center) -> 1
    glVertexAttribDivisor(2, 1); // color : one per quad -> 1
    glVertexAttribDivisor(3, 1); // Time : one per quad -> 1

    glBindVertexArray(0);
}

void Particles::add(glm::vec3 const& pos, glm::vec3 const& speed, glm::vec4 const& color, float size, float angle, float weight, float life){

	if(allParticles_.size() < maxSize_)	allParticles_.emplace_front(pos, speed, color, size, angle, weight, life);
}

void Particles::update(float deltaTime, glm::vec3 const& camPos){

	//Peut etre enlevé
	posData_.clear();
	colorData_.clear();
	timeData_.clear();

	posData_.resize(maxSize_*4);
	colorData_.resize(maxSize_*4);
	timeData_.resize(maxSize_);

	int count = 0;

	for(auto it(allParticles_.begin()), itEnd(allParticles_.end()); it != itEnd; ){

		Particle& p(*it);

        // Decrease life
        p.life_ -= deltaTime;
        if (p.life_ > 0.0f){

            // Simulate simple physics : gravity only, no collisions
            p.speed_ += glm::vec3(0.0f,  /* -9.81f */ 0.0f, -1.5f) * (float)deltaTime * 0.5f;
            p.pos_ += p.speed_ * (float)deltaTime;
            p.cameradistance = glm::length2(p.pos_ - camPos);
            //ParticlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)deltaTime;

            // Fill the GPU buffer
            posData_[4*count+0] = p.pos_.x;
            posData_[4*count+1] = p.pos_.y;
            posData_[4*count+2] = p.pos_.z;

            posData_[4*count+3] = p.size_;

            colorData_[4*count+0] = p.color_.r;
            colorData_[4*count+1] = p.color_.g;
            colorData_[4*count+2] = p.color_.b;
            colorData_[4*count+3] = p.color_.a;

            timeData_[count] = 1.f - (p.life_/p.lifeTime_);

            ++it;

            ++count;

        }
        else{
        	//Si le temps de vie est passé, supprime la particule
            it = allParticles_.erase(it);
        }

	}

	allParticles_.sort();

}

void Particles::draw(glm::mat4 const& view, glm::mat4 const& projection){

	glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



	glUseProgram(pid_);

    glm::mat4 ViewProjectionMatrix = projection * view;


    // Same as the billboards tutorial
    glUniform3f(glGetUniformLocation(pid_, "CameraRight_worldspace"), view[0][0], view[1][0], view[2][0]);
    glUniform3f(glGetUniformLocation(pid_, "CameraUp_worldspace")   , view[0][1], view[1][1], view[2][1]);

    glUniformMatrix4fv(glGetUniformLocation(pid_, "VP"), 1, GL_FALSE, &ViewProjectionMatrix[0][0]);

	size_t count(posData_.size());

    glBindBuffer(GL_ARRAY_BUFFER, posVbo_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(float), posData_.data());

    glBindBuffer(GL_ARRAY_BUFFER, colorVbo_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(float), colorData_.data());

    glBindBuffer(GL_ARRAY_BUFFER, timeVbo_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, timeData_.size() * sizeof(float), timeData_.data());

    //La texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId_);

    glBindVertexArray(vao_);

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, count);
}