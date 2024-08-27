class BoxCollider
{
private:
    int left;
    int right;
    int top;
    int bottom;
public:
    BoxCollider() : left(0) , right(0) , top(0) , bottom(0) {}
    BoxCollider(int left, int right, int top, int bottom)
        :
        left(left), right(right), top(top), bottom(bottom) {}
public:
    bool IsCollidingWith(const BoxCollider collider) const
    {
        bool x_overlap = left < collider.right && right > collider.left;
        bool y_overlap = top < collider.bottom && bottom > collider.top;
        return x_overlap && y_overlap;
    }
public:
    int GetLeft() const
    {
        return left;
    }
    int GetRight() const
    {
        return right;
    }
    int GetTop() const
    {
        return top;
    }
    int GetBottom() const
    {
        return bottom;
    }
public:
    int GetArea() const
    {
        return (right - left) * (bottom - top);
    }
};