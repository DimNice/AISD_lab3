#pragma once
#include <complex>
#include <iostream>
#include <locale.h>
#include <vector>
#include <math.h>

struct IntPoint2
{
    int _x;
    int _y;


    bool operator== (const IntPoint2& rhs) const noexcept
    {
        if ((_x == rhs._x) && (_y == rhs._y)) return true;
        else return false;
    }

    float Distance(const IntPoint2& rhs) const noexcept
    {
        return static_cast<float>(sqrt(pow((_x - rhs._x), 2) + pow((_y - rhs._y), 2)));
    }
};

std::ostream& operator<< (std::ostream& out, const IntPoint2& point)
{
    out << "(" << point._x << "; " << point._y << ")";
    return out;
}

std::istream& operator>> (std::istream& in, IntPoint2& point)
{
    in >> point._x;
    in >> point._y;
    return in;
}

struct DoublePoint3
{
    double _x;
    double _y;
    double _z;


    bool operator== (const DoublePoint3& rhs) const noexcept
    {
        if ((_x == rhs._x) && (_y == rhs._y) && (_z == rhs._z)) return true;
        else return false;
    }

    double Distance(const DoublePoint3& rhs) const noexcept
    {
        return sqrt(pow((_x - rhs._x), 2) + pow((_y - rhs._y), 2) + pow((_z - rhs._z), 2));
    }
};

std::ostream& operator<< (std::ostream& out, const DoublePoint3& point)
{
    out << "(" << point._x << "; " << point._y << "; " << point._z << ")";
    return out;
}

std::istream& operator>> (std::istream& in, DoublePoint3& point)
{
    in >> point._x;
    in >> point._y;
    in >> point._z;
    return in;
}


template <class T>
class Polyline
{
    std::vector<T> v;

public:
    Polyline() = default;
    Polyline(const Polyline<T>& polyline) = default;
    Polyline& operator = (const Polyline& line) = default;
    size_t Vertex() const;
    auto Length() const;
    bool operator== (const Polyline& rhs) const;
    bool operator!= (const Polyline& rhs) const;
    Polyline operator+ (const Polyline& polyline);
    T operator[] (const size_t index) const;
    T& operator[] (const size_t index);
    void AddToEnd(T& point);
    void AddToBegin(T& point);
    auto begin();
    auto end();
};

template <class T>
std::ostream& operator<< (std::ostream& out, const Polyline<T>& polyline)
{
    out << "Polyline[";
    for (size_t i = 0; i < polyline.Vertex(); ++i)
    {
        if (i != polyline.Vertex() - 1) out << polyline[i] << ", ";
        else out << polyline[i];
    }
    out << "]";
    return out;
}

template<class T>
size_t Polyline<T>::Vertex() const
{
    return v.size();
}

template <class T>
auto Polyline<T>::Length() const
{
    if ((v.empty()) || (v.size() < 2)) throw std::logic_error("Ломанная пуста");
    auto length = v[0].Distance(v[1]);
    for (size_t i = 1; i < v.size() - 1; ++i)
    {
        length += v[i].Distance(v[i + 1]);
    }
    return length;
}

template <>
auto Polyline<std::complex<double>>::Length() const
{
    if ((v.empty()) || (v.size() < 2)) throw std::logic_error("Ломанная пуста");
    double length = 0;
    for (size_t i = 0; i < v.size() - 1; ++i)
    {
        length += std::abs((v[i + 1] - v[i]));
    }
    return length;
}

template <class T>
bool Polyline<T>::operator== (const Polyline<T>& rhs) const
{
    if (v.size() != rhs.v.size()) return false;
    for (size_t i = 0; i < v.size(); ++i)
    {
        if (!(v[i] == rhs[i])) return false;
    }
    return true;
}

template <class T>
bool Polyline<T>::operator!= (const Polyline<T>& rhs) const
{
    if (v.size() != rhs.v.size()) return true;
    for (size_t i = 0; i < v.size(); ++i)
    {
        if (!(v[i] == rhs[i])) return true;
    }
    return false;
}

template <class T>
Polyline<T> Polyline<T>::operator+ (const Polyline<T>& polyline)
{
    if (v.empty()) return polyline;
    if (!polyline.Vertex()) return *this;
    Polyline<T> result(*this);
    result.v.resize(Vertex() + polyline.Vertex());
    for (size_t i = 0; i < polyline.v.size(); ++i)
    {
        result.v[v.size() + i] = polyline.v[i];
    }
    return result;
}

template <class T>
T Polyline<T>::operator[] (const size_t index) const
{
    if (index >= this->Vertex()) throw std::out_of_range("Неправильный индекс");
    return v.at(index);
}

template <class T>
T& Polyline<T>::operator[] (const size_t index)
{
    if (index >= this->Vertex()) throw std::out_of_range("Неправильный индекс");
    return v.at(index);
}

template <class T>
void Polyline<T>::AddToEnd(T& point)
{
    for (auto it : *this)
    {
        if (it == point) throw std::logic_error("Добавление существующей точки");
    }
    v.push_back(point);
}

template <class T>
void Polyline<T>::AddToBegin(T& point)
{
    for (auto it : *this)
    {
        if (it == point) throw std::logic_error("Добавление существующей точки");
    }
    v.insert(v.begin(), point);
}

template <class T>
auto Polyline<T>::begin()
{
    return v.begin();
}

template <class T>
auto Polyline<T>::end()
{
    return v.end();
}



template<class T>
void CheckInputValue(T& value)
{
    while (true)
    {
        std::cin >> value;
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Введеное значение неверно" << std::endl;
        }
        else
        {
            return;
        }
    }
}


int Menu1()
{
 
    std::cout << "\nИзменить первую ломанную - 1\nИзменить вторую ломанную - 2\nСложить две ломанные - 3\nВывести ломанные - 4\nПроверка ломаных на равенство - 5\nВыход - 0" << std::endl;
    while (true)
    {
        int value;
        CheckInputValue(value);
        if (value == 1 || value == 2 || value == 3 || value == 4 || value == 5 || value == 0) return value;
    }
}


int Menu2()
{
    std::cout << "\nДобавить точку в начало ломанной - 1\nДобавить точку в конец ломанной - 2\nИзменить точку по индексу - 3\nДлина ломанной - 4\nВыход - 0" << std::endl;
    while (true)
    {
        int value;
        CheckInputValue(value);
        if (value == 1 || value == 2 || value == 3 || value == 4 || value == 0) return value;
    }
}


int MainMenu()
{
    
    std::cout << "Выберите тип точек ломанной:" << std::endl;
    std::cout << "\nIntPoint2 - 1\nDoublePoint3 - 2\nstd::complex<double> - 3\nExit - 0" << std::endl;
    while (true)
    {
        int value;
        CheckInputValue(value);
        if (value == 1 || value == 2 || value == 3 || value == 0) return value;
    }
}


template <class T>
void EditPolyline(Polyline<T>& polyline)
{
    bool repeat = true;
    while (repeat)
    {
        std::cout << "\n" << polyline << std::endl;
        int m2 = Menu2();
        switch (m2) {
        case 1: {
            T point;
            std::cout << "Введите координаты точки: " << std::endl;
            std::cin >> point;
            try {
                polyline.AddToBegin(point);
            }
            catch (std::exception& err) {
                std::cout << err.what() << std::endl;
            }
            break;
        }

        case 2: {
            T point;
            std::cout << "Введите координаты точки:" << std::endl;
            std::cin >> point;
            try {
                polyline.AddToEnd(point);
            }
            catch (std::exception& err) {
                std::cout << err.what() << std::endl;
            }
            break;
        }
        case 3: {
            std::cout << "Введите индекс: ";
            int index = 0;
            CheckInputValue(index);
            try {
                std::cout << polyline[index] << std::endl;
                T point;
                std::cout << "Введите координаты точки:" << std::endl;
                std::cin >> point;
                polyline[index] = point;
            }
            catch (std::exception& err) {
                std::cout << err.what() << std::endl;
            }
            break;
        }
        case 4: {
            try {
                std::cout << "Длина ломанной: " << polyline.Length() << std::endl;
                std::cout << typeid(polyline.Length()).name() << std::endl;
            }
            catch (std::exception& err) {
                std::cout << err.what() << std::endl;
            }
            break;
        }
        case 0: {
            repeat = false;
            break;
        }
        default:
            break;

        }
    }
}


void EditPolyline(Polyline<std::complex<double>>& polyline)
{
    bool repeat = true;
    while (repeat)
    {
        std::cout << "\n" << polyline << std::endl;
        int m2 = Menu2();
        switch (m2) {
        case 1: {
            double im = 0, re = 0;
            std::cout << "Введите координаты точки:" << std::endl;
            std::cout << "Действительная часть(re): ";
            std::cin >> re;
            std::cout << "Мнимая часть(im): ";
            std::cin >> im;
            std::complex<double> point(re, im);
            try {
                polyline.AddToBegin(point);
            }
            catch (std::exception& err) {
                std::cout << err.what() << std::endl;
            }
            break;
        }

        case 2: {
            double im = 0, re = 0;
            std::cout << "Введите координаты точки:" << std::endl;
            std::cout << "Действительная часть(re): ";
            std::cin >> re;
            std::cout << "Мнимая часть(im): ";
            std::cin >> im;
            std::complex<double> point(re, im);
            try {
                polyline.AddToEnd(point);
            }
            catch (std::exception& err) {
                std::cout << err.what() << std::endl;
            }
            break;
        }
        case 3: {
            std::cout << "Введите индекс: ";
            int index = 0;
            CheckInputValue(index);
            try {
                std::cout << polyline[index] << std::endl;
                double im = 0, re = 0;
                std::cout << "Введите координаты точки:" << std::endl;
                std::cout << "Действительная часть(re): ";
                std::cin >> re;
                std::cout << "Мнимая часть(im): ";
                std::cin >> im;
                std::complex<double> point(re, im);
                polyline[index] = point;
            }
            catch (std::exception& err) {
                std::cout << err.what() << std::endl;
            }
            break;
        }
        case 4: {
            try {
                std::cout << "Длина ломанной: " << polyline.Length() << std::endl;
                std::cout << typeid(polyline.Length()).name() << std::endl;
            }
            catch (std::exception& err) {
                std::cout << err.what() << std::endl;
            }
            break;
        }
        case 0: {
            repeat = false;
            break;
        }
        default:
            break;

        }
    }
}


int main()
{
    setlocale(LC_ALL, "Russian");
    bool end = false;
    while (!end)
    {
        int option = MainMenu();
        switch (option)
        {
        case 1: {
            bool repeat = true;
            Polyline<IntPoint2> polyline1;
            Polyline<IntPoint2> polyline2;
            while (repeat) {
                int m1 = Menu1();
                switch (m1) {
                case 1: {
                    EditPolyline(polyline1);
                    break;
                }
                case 2: {
                    EditPolyline(polyline2);
                    break;
                }
                case 3: {
                    Polyline<IntPoint2> res;
                    res = polyline1 + polyline2;
                    std::cout << res << std::endl;
                    break;
                }
                case 4: {
                    std::cout << "Первая ломанная: " << polyline1 << std::endl;
                    std::cout << "Вторая ломанная: " << polyline2 << std::endl;
                    break;
                }
                case 5: {
                    if (polyline1 == polyline2) std::cout << "Ломанные равны" << std::endl;
                    else std::cout << "Ломанные не равны" << std::endl;
                    break;
                }
                case 0: {
                    repeat = false;
                    break;
                }
                default:
                    break;
                }
            }
            break;
        }

        case 2: {
            bool repeat = true;
            Polyline<DoublePoint3> polyline1;
            Polyline<DoublePoint3> polyline2;
            while (repeat) {
                int m1 = Menu1();
                switch (m1) {
                case 1: {
                    EditPolyline(polyline1);
                    break;
                }
                case 2: {
                    EditPolyline(polyline2);
                    break;
                }
                case 3: {
                    Polyline<DoublePoint3> res;
                    res = polyline1 + polyline2;
                    std::cout << res << std::endl;
                    break;
                }
                case 4: {
                    std::cout << "Первая ломанная: " << polyline1 << std::endl;
                    std::cout << "Вторая ломанная: " << polyline2 << std::endl;
                    break;
                }
                case 5: {
                    if (polyline1 == polyline2) std::cout << "Ломанные равны" << std::endl;
                    else std::cout << "Ломанные не равны" << std::endl;
                    break;
                }
                case 0: {
                    repeat = false;
                    break;
                }
                default:
                    break;
                }
            }
            break;
        }

        case 3: {
            bool repeat = true;
            Polyline<std::complex<double>> polyline1;
            Polyline<std::complex<double>> polyline2;
            while (repeat) {
                int m1 = Menu1();
                switch (m1) {
                case 1: {
                    EditPolyline(polyline1);
                    break;
                }
                case 2: {
                    EditPolyline(polyline2);
                    break;
                }
                case 3: {
                    Polyline<std::complex<double>> res;
                    res = polyline1 + polyline2;
                    std::cout << res << std::endl;
                    break;
                }
                case 4: {
                    std::cout << "Первая ломанная: " << polyline1 << std::endl;
                    std::cout << "Вторая ломанная: " << polyline2 << std::endl;
                    break;
                }
                case 5: {
                    if (polyline1 == polyline2) std::cout << "Ломанные равны" << std::endl;
                    else std::cout << "Ломанные не равны" << std::endl;
                    break;
                }
                case 0: {
                    repeat = false;
                    break;
                }
                default:
                    break;
                }
            }
            break;
        }

        case 0: {
            end = true;
            break;
        }

        default: break;
        }
    }
    return 0;
}