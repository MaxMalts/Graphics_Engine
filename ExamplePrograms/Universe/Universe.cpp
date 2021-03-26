#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <Windows.h>
#undef CreateWindow
#undef max
#undef min

#include <GraphicsEngine.h>
#include "Universe.h"

namespace Universe {

	/* Vector2 */

	Vector2::Vector2(long long x, long long y) : x(x), y(y) {}


	Vector2::Vector2(Int x, Int y) : x(x), y(y) {}


	Vector2 Vector2::operator+(const Vector2& right) const {
		return Vector2(x + right.x, y + right.y);
	}

	Vector2 Vector2::operator+() const {
		return *this;
	}

	Vector2& Vector2::operator+=(const Vector2& right) {
		x += right.x;
		y += right.y;

		return *this;
	}

	Vector2 Vector2::operator-(const Vector2& right) const {
		return Vector2(x - right.x, y - right.y);
	}

	Vector2 Vector2::operator-() const {
		return Vector2(-x, -y);
	}

	Vector2& Vector2::operator-=(const Vector2& right) {
		x -= right.x;
		y -= right.y;

		return *this;
	}

	Vector2 Vector2::operator*(const Int right) const {
		return Vector2(x * right, y * right);
	}

	Vector2 operator*(const Int left, const Vector2& right) {
		return Vector2(right.x * left, right.y * left);

	}

	Vector2 Vector2::operator/(const Int right) const {
		return Vector2(x / right, y / right);
	}



	/* Additional */

	Int ClassicDistance(const Vector2& firstPoint, const Vector2& secondPoint) {
		return sqrt((secondPoint.x - firstPoint.x) * (secondPoint.x - firstPoint.x) +
			(secondPoint.y - firstPoint.y) * (secondPoint.y - firstPoint.y));
	}



	/* Particle */

	Particle::Particle(Universe* universe, Vector2 position, const Int mass,
		Vector2 velocity, const int visualRadius)
		: universe(universe), pos(position), mass(mass), vel(velocity), visualRad(visualRadius) {}


	Particle::Particle(const Particle& other) {
		Copy(other);
	}


	void Particle::Draw() {
		Vector2 visualPos = pos / universe->VisualDownscale();

		visualPoint = dynamic_cast<GUI::Point*>(universe->Space()->CreatePrimitive(
			GUI::Primitive::point, GUI::PointProps(GUI::Vector2(visualPos.x, visualPos.y), visualRad, GUI::Color::white)
		));
	}


	void Particle::AddForce(const Vector2& force) {
		this->force += force;
	}


	Vector2 Particle::Position() const {
		return pos;
	}


	Int Particle::Mass() const {
		return mass;
	}


	void Particle::UpdateState() {
		vel += force / mass;
		pos += vel;

		if (pos.x < Int(0)) {
			pos.x = universe->Size().x;
		} else if (pos.x > universe->Size().x) {
			pos.x = 0;
		}

		if (pos.y < Int(0)) {
			pos.y = universe->Size().y;
		} else if (pos.y > universe->Size().y) {
			pos.y = 0;
		}

		if (visualPoint != nullptr) {
			Vector2 visualPos = pos / universe->VisualDownscale();

			visualPoint->ChangePosition(GUI::Vector2(visualPos.x, visualPos.y));
		}

		force.x = 0;
		force.y = 0;
	}


	Particle& Particle::operator=(const Particle& other) {
		if (visualPoint != nullptr) {
			Undraw();
		}

		Copy(other);

		return *this;
	}


	void Particle::Undraw() {
		if (visualPoint != nullptr) {
			universe->Space()->RemovePrimitive(visualPoint);
		}
	}


	void Particle::Copy(const Particle& other) {
		universe = other.universe;

		pos = other.pos;
		vel = other.vel;
		mass = other.mass;

		force = other.force;

		visualRad = other.visualRad;
	}



	/* Universe */

	Universe::Universe(const Vector2& universeSize, const double visualDownscale,
	                   const long long nParticles, const int refreshTimeMs)
		: size(universeSize), visualDownscale(visualDownscale), refreshTimeMs(refreshTimeMs) {
		
		Vector2 visualSize = size / visualDownscale;

		osWindow = application.CreateWindow(GUI::Vector2(visualSize.x, visualSize.y), "Universe", GUI::Color::black);
		space = osWindow->GetDesktop();
		space->AddEventListener(GUI::Event::window_close, GUI::OSWindowCloseListener);

		for (long long i = 0; i < nParticles; ++i) {
			Vector2 offset(std::rand() % nParticles, std::rand() % nParticles);
			Vector2 velocity(std::rand() % 1, std::rand() % 1);
			int mass = std::rand() % 3 + 1;

			particles.emplace_back(this, size / 2 + offset, mass, velocity * -10, mass * 2);
		}
		for (Particle& curParticle : particles) {
			curParticle.Draw();
		}

		//particles[0].mass = 10;
		//particles[0].pos = size / 2;
		//particles[0].vel = Vector2(0, -100);

		//particles[1].mass = 10;
		//particles[1].pos = Vector2(size.x / 2 - Int(1000), size.y / 2);
		//particles[1].vel = Vector2(0, 100);
	}


	void Universe::Start() {
		double prevTime = glfwGetTime();
		while (application.WindowsOpened() > 0) {
			ProcessTick();

			application.UpdateAllWindows();
			application.ProcessEvents();
			double curTime = glfwGetTime();
			std::cout << static_cast<int>(1 / (curTime - prevTime)) << '\n';
			prevTime = curTime;

			if (refreshTimeMs > 0) {
				Sleep(refreshTimeMs);
			}
		}
	}


	GUI::Window* Universe::Space() const {
		return space;
	}


	Vector2 Universe::Size() const {
		return size;
	}


	double Universe::VisualDownscale() const {
		return visualDownscale;
	}


	void Universe::ProcessTick() {
		ProcessForces();

		for (Particle& curParticle : particles) {
			curParticle.UpdateState();
		}
	}


	void Universe::ProcessForces() {
		for (long long i = 0; i < particles.size() - 1; ++i) {
			for (long long j = i + 1; j < particles.size(); ++j) {
				Particle& particle1 = particles[i];
				Particle& particle2 = particles[j];

				Vector2 pos1 = particle1.Position();
				Vector2 pos2 = particle2.Position();

				Vector2 firstParticleForce(0, 0);

				Int distance = Distance(pos1, pos2);

				if (0 == distance) {
					firstParticleForce = Vector2(particle1.Mass() * particle2.Mass(), Int(0));

				} else {
					firstParticleForce += Gravitation(particle1, particle2, distance);
					firstParticleForce += StrongForce(particle1, particle2, distance);
				}

				particle1.AddForce(firstParticleForce);
				particle2.AddForce(-firstParticleForce);
			}
		}
	}


	Vector2 Universe::Gravitation(Particle& first, Particle& second, Int distance) {
		return gravityConstant * first.Mass() * second.Mass() *
		       (second.Position() - first.Position()) / distance / distance / distance;
	}


	Vector2 Universe::StrongForce(Particle& first, Particle& second, Int distance) {
		return -strongForceConstant * first.Mass() * second.Mass() * (1 - distance / Int((strongRadConstant + 10))) *
			(second.Position() - first.Position()) / distance;
	}
}