#include "World.h"

World::World()
{
	//world_ = &(std::make_unique<b2World>(b2Vec2(0, 0)));
	//world_ = &b2World(b2Vec2(0, 0));
}

b2Body* World::createBody(Piece& piece)
{
	std::vector<b2Vec2> b2Poly;
	int numCoords = piece.getNumCoords();

	for (int i = 0; i < numCoords; i++)
	{
		auto [x, y] = piece.getVertexCoord(i);
		float x_ = static_cast<float>(x);
		float y_ = static_cast<float>(y);
		b2Poly.push_back(b2Vec2{ x_,y_});
	}

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = b2Vec2{0,0};
	//bodyDef.position.Set(0.0f, 4.0f);

	b2PolygonShape shape;
	shape.Set(b2Poly.data(), b2Poly.size());
	/*if (is_polygon_degenerate(points)) {
		s.SetAsBox(b2_linearSlop * 2,
			b2_linearSlop * 2);
	}
	else {
		s.Set(points.data(), points.size());
	}*/

	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1;
	fixture.filter.groupIndex = -2;

	b2Body* oBody = world_.CreateBody(&bodyDef);
	oBody->CreateFixture(&fixture);
	return oBody;
}


void World::Init(std::vector<Piece>& pieces)
{
	for (auto pieceIt = pieces.begin(); pieceIt!=pieces.end(); pieceIt++)
	{
		b2Body* body = this->createBody(*pieceIt);
		pieceIt->refb2Body_ = body;
		bodies_.push_back(body);
	}

	// TODO call connectSpringsToPieces by the pairs


}

void World::connectSpringsToPieces(const EdgeMating &edgeMating)
{

}