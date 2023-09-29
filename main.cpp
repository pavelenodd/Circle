#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

//  Базовый класс для 3D-кривых
class Curve {
 public:
  Curve() {}
  virtual ~Curve() {}

  virtual double GetRadius() const = 0;
  virtual double GetStep() const = 0;
  virtual double CalculateX(double t) const = 0;
  virtual double CalculateY(double t) const = 0;
  virtual double CalculateZ(double t) const = 0;
  virtual double CalculateDX(double t) const = 0;
  virtual double CalculateDY(double t) const = 0;
  virtual double CalculateDZ(double t) const = 0;
};
// Глоб пререм
float PI = 3.14f;

// Класс для окружности
class Circle : public Curve {
 private:
  double radius;

 public:
  Circle(double r) : radius(r) {}

  double GetRadius() const override { return radius; }

  double GetStep() const override {
    return 0.0;  // Окружность не имеет шага
  }

  double CalculateX(double t) const override { return radius * cos(t); }

  double CalculateY(double t) const override { return radius * sin(t); }

  double CalculateZ(double t) const override {
    return 0.0;  // Окружность в плоскости XOY
  }

  double CalculateDX(double t) const override { return -radius * sin(t); }

  double CalculateDY(double t) const override { return radius * std::cos(t); }

  double CalculateDZ(double t) const override { return 0.0; }
};

// Класс для эллипса
class Ellipse : public Curve {
 private:
  double radiusX;
  double radiusY;

 public:
  Ellipse(double rx, double ry) : radiusX(rx), radiusY(ry) {}

  double GetRadius() const override {
    return max(radiusX, radiusY);  // Вернуть больший радиус для эллипса
  }

  double GetStep() const override {
    return 0.0;  // Эллипс не имеет шага
  }

  double CalculateX(double t) const override { return radiusX * cos(t); }

  double CalculateY(double t) const override { return radiusY * sin(t); }

  double CalculateZ(double t) const override {
    return 0.0;  // Эллипс в плоскости XOY
  }

  double CalculateDX(double t) const override { return -radiusX * sin(t); }

  double CalculateDY(double t) const override { return radiusY * cos(t); }

  double CalculateDZ(double t) const override { return 0.0; }
};

// Класс для трехмерной спирали
class Spiral : public Curve {
 private:
  double radius;
  double step;

 public:
  Spiral(double r, double s) : radius(r), step(s) {}

  double GetRadius() const override { return radius; }

  double GetStep() const override { return step; }

  double CalculateX(double t) const override { return radius * cos(t); }

  double CalculateY(double t) const override { return radius * sin(t); }

  double CalculateZ(double t) const override {
    return step * t / (2 * PI);  // Формула для Z
  }

  double CalculateDX(double t) const override { return -radius * sin(t); }

  double CalculateDY(double t) const override { return radius * ::cos(t); }

  double CalculateDZ(double t) const override { return step / (2 * PI); }
};

int main() {
  // Создаем контейнер объектов кривых
  vector<unique_ptr<Curve>> curves;

  // Генерируем случайные кривые
  curves.push_back(make_unique<Circle>(5.0));
  curves.push_back(make_unique<Ellipse>(3.0, 2.0));
  curves.push_back(make_unique<Spiral>(4.0, 1.0));

  // Выводим координаты точек и производные кривых при t=PI/4
  double t = PI / 4;
  setlocale(LC_ALL, "Russian");
  for (const auto& curve : curves) {
    cout << "тип кривой: ";
    if (dynamic_cast<Circle*>(curve.get())) {
      cout << "круг";
    } else if (dynamic_cast<Ellipse*>(curve.get())) {
      cout << "элипс";
    } else if (dynamic_cast<Spiral*>(curve.get())) {
      cout << "спираль";
    }
    cout << endl;
    cout << "точки: (" << curve->CalculateX(t) << ", " << curve->CalculateY(t)
         << ", " << curve->CalculateZ(t) << ")" << endl;
    cout << "производная: (" << curve->CalculateDX(t) << ", "
         << curve->CalculateDY(t) << ", " << curve->CalculateDZ(t) << ")"
         << endl;
  }

  // Создаем второй контейнер для кругов
  vector<const Circle*> circles;

  // Заполняем второй контейнер только кругами из первого контейнера
  for (const auto& curve : curves) {
    if (dynamic_cast<Circle*>(curve.get())) {
      circles.push_back(static_cast<Circle*>(curve.get()));
    }
  }

  // Сортируем второй контейнер в порядке возрастания радиусов окружностей
  sort(circles.begin(), circles.end(), [](const Circle* a, const Circle* b) {
    return a->GetRadius() < b->GetRadius();
  });

  // Вычисляем общую сумму радиусов всех кругов во втором контейнере
  double totalRadiusSum = 0.0;
  for (const Circle* circle : circles) {
    totalRadiusSum += circle->GetRadius();
  }

  cout << "Total radius sum of circles: " << totalRadiusSum << endl;

  return 0;
}
