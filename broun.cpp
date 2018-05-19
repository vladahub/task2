#include <iostream>
#include <vector>
#include <math.h>

#include "GLBall.hpp"
#include "NBodyScene.hpp"

#define WALL1  0.0
#define WALL2  0.0
#define WALL3  700.0
#define WALL4  700.0
#define m 5.0
#define M 10.0

class SampleBall : public GLBall
{
protected:
    double vx;
    double vy;
    double x;
    double y;
    double r;

public:
	SampleBall(double x, double y, double r, double vx, double vy) {
		this->x = x;
		this->y = y;
		this->vx = vx;
		this->vy = vy;
		this->r = r;
	}

	double getX() const override{
		return x;
	}

	double getY() const override{
		return y;
	}
	
	double getR() const override{
		return r;
	}
	
	double getMass() {
		return M;
	}
	double getVx() {
		return vx;
	}

	double getVy() {
		return vy;
	}

	double getSpeed() {
		return sqrt(vx*vx + vy*vy);
	}

	void move(double dt) {
		 ((x + vx + r >= WALL3 || x + vx - r <= WALL1)? (vx = -vx) : (x += vx * dt));
		 ((y + vy + r >= WALL4 || y + vy - r <= WALL2)? (vy = -vy) : (y += vy * dt));
	}

	void setVx(double vx) {
		this->vx = vx;
	}

	void setVy(double vy) {
		this->vy = vy;
	}

	~SampleBall() {}

};

std::ostream& operator<<(std::ostream& osB, SampleBall& B) {
   osB << "SampleBall (" << B.getX() << ", " << B.getY() << "), "
	   << "R: " << B.getR() << ", "
	   << "V: " << B.getSpeed() << ", "
	   << "M: " << B.getMass() << std::endl;
	return osB;
}

class SampleScene : public NBodyScene {
protected:
    std::vector<SampleBall> bodies;

public:
    unsigned int getNumberOfBodies() const override {
        return bodies.size();
    }

    const GLBall& getBody(unsigned int number) const override {  
        return bodies.at(number);
    }

    void doTimeStep() override {
        checkCollision();
    }

    void initScene() {
        int n;
        double x, y, r, vx, vy;

        std::cin >> n;

        for(int i = 0; i < n; i++) {
	        std::cin >> x;
	        std::cin >> y;
	        std::cin >> r;
	        std::cin >> vx;
	        std::cin >> vy;

	        bodies.push_back(SampleBall(x, y, r, vx, vy));
    	}

        std::cout << bodies.at(0);
	}

// если все еще слишком близко шарики блинкануть
    void checkCollision() {
		for(auto it = bodies.begin() + 1; it < bodies.end(); it++)
		{
			double s = sqrt(pow(((*it).getX() - bodies.at(0).getX()), 2) + pow(((*it).getY() - bodies.at(0).getY()), 2));
			std::cout << s << "<= " << (*it).getR() + bodies.at(0).getR() << "\n";
			if(s <= ((*it).getR() + bodies.at(0).getR()))
			{
				double alpha = atan(abs((*it).getX() - bodies.at(0).getX()) / abs((*it).getY() - bodies.at(0).getY()));
				double m1 = m;
				double m2 = M;
				double vx1 = (*it).getVx();
				double vy1 = (*it).getVy();
				double vx2 = bodies.at(0).getVx();
				double vy2 = bodies.at(0).getVy();

				double vx1_after = ((((m1 - m2) * (vx1*cos(alpha) + vy1*sin(alpha)) + 2*m2*(vx2*cos(alpha) + vy2*sin(alpha))) * cos(alpha)) / (m1 + m2)) 
				+ ((vy1*cos(alpha) - vx1*sin(alpha)) * cos(alpha + 3.14/2));

				double vy1_after = ((((m1 - m2) * (vx1*cos(alpha) + vy1*sin(alpha)) + 2*m2*(vx2*cos(alpha) + vy2*sin(alpha))) * sin(alpha)) / (m1 + m2)) 
				+ ((vy1*cos(alpha) - vx1*sin(alpha)) * sin(alpha + 3.14/2));

				(*it).setVx(vx1_after);
				(*it).setVy(vy1_after);

				double vx2_after = ((((m2 - m1) * (vx2*cos(alpha) + vy2*sin(alpha)) + 2*m1*(vx1*cos(alpha) + vy1*sin(alpha))) * cos(alpha)) / (m1 + m2)) 
				+ ((vy2*cos(alpha) - vx2*sin(alpha)) * cos(alpha + 3.14/2));

				double vy2_after = ((((m2 - m1) * (vx2*cos(alpha) + vy2*sin(alpha)) + 2*m1*(vx1*cos(alpha) + vy1*sin(alpha))) * sin(alpha)) / (m1 + m2)) 
				+ ((vy2*cos(alpha) - vx2*sin(alpha)) * sin(alpha + 3.14/2));

				bodies.at(0).setVx(vx2_after);
				bodies.at(0).setVy(vy2_after);
			}

			(*it).move(0.1);

			std::cout << "Big " << bodies.at(0) << std::endl;
			std::cout << (*it).getX() << std::endl;
			std::cout << (*it).getY() << std::endl;
		}

		bodies.at(0).move(0.1);
	}
};

NBodyScene* getScene()
{
    SampleScene* s = new SampleScene();
    s->initScene();
    return s;
}

int main()
{
	SampleScene* S = (SampleScene*)getScene();

	while(true) {
		S->doTimeStep();
	}

	delete S;

	return 0;
}
