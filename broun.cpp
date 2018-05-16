#include <iostream>
#include <vector>
#include <math.h>


#define WALL1  0.0
#define WALL2  0.0
#define WALL3  700.0
#define WALL4  700.0

class GLBall
{
public:
	virtual double getX() = 0;
	virtual double getY() = 0;
	virtual double getR() = 0;
	virtual ~GLBall() {}
};

class BigBro : public GLBall
{
private: double x, y, R, Vx, Vy, M;
public:
	BigBro() {
		x = 200;
		y = 200;
		Vx = 0;
		Vy = 0;
		R = 20;
		M = 100; 
	}

	BigBro(double x, double y, double R, double Vx, double Vy, double M) {
		this->x = x;
		this->y = y;
		this->Vx = Vx;
		this->Vy = Vy;
		this->R = R;
		this->M = M;
	}

	double getX() {
		return x;
	}

	double getY() {
		return y;
	}
	
	double getR() {
		return R;
	}
	
	double getMass() {
		return M;
	}
	double getVx() {
		return Vx;
	}

	double getVy() {
		return Vy;
	}

	double getSpeed() {
		return sqrt(Vx*Vx + Vy*Vy);
	}
	double getImpulse() {
		return getSpeed() * M;
	}

	void move(double dt) {
		((abs(x - WALL1) <= R || abs(x - WALL3) <= R)? (x += -Vx * dt) : (x += Vx * dt));
		((abs(y - WALL2) <= R || abs(y - WALL3) <= R)? (y += -Vy * dt) : (y += Vy * dt));
	}

	void setVx(double Vx) {
		this->Vx = Vx;
	}

	void setVy(double Vy) {
		this->Vy = Vy;
	}

	~BigBro() {}

};

std::ostream& operator<<(std::ostream& osB, BigBro& B) {
   osB << "BigBro (" << B.getX() << ", " << B.getY() << "), "
	   << "R: " << B.getR() << ", "
	   << "V: " << B.getSpeed() << ", "
	   << "M: " << B.getMass() << std::endl;
	return osB;
}

class LilBro : public GLBall
{
private: double x, y, r, vx, vy, m;
public:
	LilBro() {
		x = 30;
		y = 30;
		r = 10;
		vx = rand();
		vy = rand();
		m = 10;
	}

	LilBro(double x, double y, double r, double vx, double vy, double m) {
		this->x = x;
		this->y = y;
		this->r = r;
		this->vx = vx;
		this->vy = vy;
		this->m = m;
	}

	double getX() {
		return x;
	}

	double getY() {
		return y;
	}
	
	double getvX() {
		return vx;
	}
	
	double getvY() {
		return vy;
	}
	
	double getR() {
		return r;
	}
	
	double getmass() {
		return m;
	}
	
	double getSpeed() {
		return sqrt(vx*vx + vy*vy);
	}
	double getImpulse() {
		return getSpeed() * m;
	}

	void setvX(double vx) {
		this->vx = vx;
	}

	void setvY(double vy) {
		this->vy = vy;
	}

	void move(double dt) {
	 ((abs(x - WALL1) <= r || abs(x - WALL3) <= r)? (x += -vx * dt) : (x += vx * dt));
	 ((abs(y - WALL2) <= r || abs(y - WALL3) <= r)? (y += -vy * dt) : (y += vy * dt));
	}


	~LilBro() {}
};

class NBodyScene {
public:
    virtual ~NBodyScene() = default;
    virtual unsigned int getNumberOfBros() const = 0;
    virtual const GLBall& getBro(unsigned int number) const = 0;
    virtual void doTimeStep() = 0;
};

class SampleScene : public NBodyScene {
protected:
    std::vector<LilBro> bros;
    BigBro Big;

public:
    unsigned int getNumberOfBros() const override {
        return bros.size() + 1;
    }

    const GLBall& getBro(unsigned int number) const override {  
        if (number == 1) return Big;
        return bros.at(number);
    }

    void doTimeStep() override {
        for(LilBro& b : bros) {
            b.move(0.1);
        	std::cout << "Lil.v: " << b.getSpeed() << std::endl;
        }

        Big.move(0.1);
        std::cout << "Big.V: " << Big.getSpeed() << std::endl;
    }

    void initScene() {
        bros.push_back(LilBro(179, 200, 1, 1, 1, 10));
        BigBro B;
	    }

    void checkCollision() {
		for(auto it = bros.begin(); it < bros.end(); it++)
		{
			double s = sqrt(pow(((*it).getX() - Big.getX()), 2) + pow(((*it).getY() - Big.getY()), 2));
			std::cout << s << "<= " << (*it).getR() + Big.getR() << "\n";
			if(s <= (*it).getR() + Big.getR())
			{
				double alpha = atan(abs((*it).getX() - Big.getX()) / abs((*it).getY() - Big.getY()));
				double m1 = (*it).getmass();
				double m2 = Big.getMass();
				double vx1 = (*it).getvX();
				double vy1 = (*it).getvY();
				double vx2 = Big.getVx();
				double vy2 = Big.getVy();

				double vx1_after = ((((m1 - m2) * (vx1*cos(alpha) + vy1*sin(alpha)) + 2*m2*(vx2*cos(alpha) + vy2*sin(alpha))) * cos(alpha)) / (m1 + m2)) 
				+ ((vy1*cos(alpha) - vx1*sin(alpha)) * cos(alpha + 3.14/2));

				double vy1_after = ((((m1 - m2) * (vx1*cos(alpha) + vy1*sin(alpha)) + 2*m2*(vx2*cos(alpha) + vy2*sin(alpha))) * sin(alpha)) / (m1 + m2)) 
				+ ((vy1*cos(alpha) - vx1*sin(alpha)) * sin(alpha + 3.14/2));

				(*it).setvX(-vx1_after);
				(*it).setvY(-vy1_after);

				double vx2_after = ((((m2 - m1) * (vx2*cos(alpha) + vy2*sin(alpha)) + 2*m1*(vx1*cos(alpha) + vy1*sin(alpha))) * cos(alpha)) / (m1 + m2)) 
				+ ((vy2*cos(alpha) - vx2*sin(alpha)) * cos(alpha + 3.14/2));

				double vy2_after = ((((m2 - m1) * (vx2*cos(alpha) + vy2*sin(alpha)) + 2*m1*(vx1*cos(alpha) + vy1*sin(alpha))) * sin(alpha)) / (m1 + m2)) 
				+ ((vy2*cos(alpha) - vx2*sin(alpha)) * sin(alpha + 3.14/2));

				Big.setVx(-vx2_after);
				Big.setVy(-vy2_after);
			}

			std::cout << "Big " << Big << std::endl;
			std::cout << (*it).getSpeed() << std::endl;
			std::cout << (*it).getvX() << std::endl;
			std::cout << (*it).getvY() << std::endl;
			std::cout << (*it).getX() << std::endl;
			std::cout << (*it).getY() << std::endl;
		}
	}
};

std::ostream& operator<<(std::ostream& os, LilBro& l) {
	os << "LilBro (" << l.getX() << ", " << l.getY() << "), "
	   << "r: " << l.getR() << ", "
	   << "v: " << l.getSpeed() << ", "
	   << "m: " << l.getmass() << std::endl;
	return os;
}

int main()
{
	SampleScene S;
 	S.initScene();

	while(true) {
		S.checkCollision();
		S.doTimeStep();
	}
	return 0;
}