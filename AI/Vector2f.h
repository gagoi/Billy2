class Vector2f{
private:
    float m_x;
    float m_y;

public:
    Vector2f();
    Vector2f(float x, float y);

    float getX();
    float getY();
    void setX(float);
    void setY(float);
}