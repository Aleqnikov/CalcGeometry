#ifndef DELAUNAYCONTROLLER_H
#define DELAUNAYCONTROLLER_H

#include "BaseController.h"
#include "../lib/src/geometry/delone/algorithm/VoronoiFromDelaunay.h"
#include "../lib/src/geometry/delone/algorithm/DelaunayTriangulation.h"

class DelaunayController : public BaseController {
	Q_OBJECT

public:
	DelaunayController(NavigationView *view, QGraphicsScene *scene,
					   const std::vector<Point2D>& vertices,
					   const std::vector<Triangle>& triangles);

	void toggleMode();
	bool isShowingVoronoi() const { return showVoronoi_; }

protected:
	void draw() override;
	void onSpacePressed() override {}

private:
	void drawDelaunay();
	void drawVoronoi();

	std::vector<Point2D>  vertices_;
	std::vector<Triangle> triangles_;
	VoronoiDiagram        voronoi_;
	bool                  showVoronoi_ = false;

	static constexpr double RAY_LENGTH = 500.0;
};

#endif