#include <LEDA/geo/circle.h>
#include <LEDA/geo/point.h>
#include <iostream>
#include <math.h>
using namespace std;
using leda::circle;
using leda::point;

#define epsilon 0.00001

circle createDiametricCircle(point a, point b) {
	point center = point((a.X() + b.X())/2, (a.Y() + b.Y())/2);
	double radius = center.distance(a);
	circle finalCircle = circle(center, radius);
	return finalCircle;
}

bool isCoveringCircle(circle c, point *p, int n) {
	int i;
	//printf("%lf,%lf\n%lf\n", c.center().X(), c.center().Y(), c.radius());
	for(i = 0; i < n; i++) {
		if (p[i].distance(c.center()) > c.radius() + epsilon) {
			//printf("Point outside is %lf,%lf\n", p[i].X(), p[i].Y());
			return false;
		}
	}
	return true;
} 

int main()
{
	int n,i,j,k,l;
	double x,y;
	point *p;
	scanf("%d", &n);
	p = new point[n];
	//printf("Are changes even reflected\n");
	for(i = 0; i < n; i++) {
		scanf("%lf %lf", &x, &y);
		p[i] = point(x,y);
	}
	// for(i = 0; i < n; i++) {
	// 	printf("%lf, %lf\n", p[i].X(), p[i].Y());
	// }

	if (n == 1) {
		printf("%lf,%lf\n%lf",p[0].X(),p[0].Y(), 0.0);
		return 0;
	}
	if (n == 2) {
		circle c = createDiametricCircle(p[0], p[1]);
		point center = c.center();
		printf("%lf,%lf\n%lf", center.X(), center.Y(), c.radius());
		return 0;
	}
	circle minCircle = circle(p[0], 1000000007);
	// printf("%lf\n", minCircle.radius());
	// printf("Only 2 points\n");
	for(i = 0; i < n-1; i++) {
		for(j = i+1; j < n;j++) {
			circle c = createDiametricCircle(p[i], p[j]);
			//printf("Checking covering circle for 2 points");
			bool covering = isCoveringCircle(c, p, n);
			bool isSmallerRadius = (c.radius() < minCircle.radius());
			//cout << covering << isSmallerRadius << endl;
			if (covering && isSmallerRadius) {
				minCircle = c;
				//printf("%lf,%lf\n%lf\n",c.center().X(),c.center().Y(),c.radius());

			}
		}
	}
	//printf("Only 3 points\n");
	for(i = 0; i < n-2; i++) {
		for(j = i+1; j < n-1;j++) {
			for(k = j+1; k < n;k++) {
				circle c = circle(p[i],p[j],p[k]);
				//printf("%lf,%lf\n%lf\n",c.center().X(),c.center().Y(),c.radius());
				//printf("%lf,%lf\n%lf\n",c.center().X(),c.center().Y(),c.radius());
				if (isCoveringCircle(c, p, n) && (c.radius() < minCircle.radius())) {
					minCircle = c;
					//printf("%lf,%lf\n%lf\n",c.center().X(),c.center().Y(),c.radius());
				}
			}
		}
	}
	point center = minCircle.center();
	double radius = minCircle.radius();
	printf("%lf,%lf\n%lf",center.X(),center.Y(), radius);
}
