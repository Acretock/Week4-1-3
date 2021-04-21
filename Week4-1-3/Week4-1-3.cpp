#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

struct Image {
    double quality;
    double freshness;
    double rating;
};

struct Params {
    double a;
    double b;
    double c;
};

class FunctionPart
{
public:
    FunctionPart(char op, double v);
    ~FunctionPart();

    double Apply(double source) const {
        if (operation == '+')
            return source + value;
        else
            return source - value;
        if (operation == '*')
            return source * value;
        else
            return source / value;
    }

    void Invert() {
        if (operation == '+')
            operation = '-';
        else
            operation = '+';

        if (operation == '*')
            operation = '/';
        else
            operation = '*';
    }

private:
    char operation;
    double value;
};

FunctionPart::FunctionPart(char op, double v)
{
    operation = op;
    value = v;
}

FunctionPart::~FunctionPart()
{
}

class Function
{
public:
    Function();
    ~Function();

    void AddPart(char op, double val) {
        parts.push_back({ op,val });
    }

    double Apply(double value) const {
        for (const FunctionPart& part : parts)
            value = part.Apply(value);
        return value;
    }

    void Invert() {
        for (FunctionPart& part : parts)
            part.Invert();
        reverse(begin(parts), end(parts));
    }

private:
    vector<FunctionPart> parts;
};

Function::Function()
{
}

Function::~Function()
{
}

Function MakeWeightFunction(const Params& params, const Image& image) {
    Function function;
    function.AddPart('*', params.a);
    function.AddPart('-', image.freshness * params.b);
    function.AddPart('+', image.rating * params.c);
    return function;
}

double ComputeImageWeight(const Params& params, const Image& image) {
    Function function = MakeWeightFunction(params, image);
    return function.Apply(image.quality);
}

double ComputeQualityByWeight(const Params& params,
    const Image& image, double weight) {
    Function function = MakeWeightFunction(params, image);
    function.Invert();
    return function.Apply(weight);
}

int main()
{
    Image image = { 10,2,6 };
    Params params = { 4,2,6 };
    cout << ComputeImageWeight(params, image) << endl;
    cout << ComputeQualityByWeight(params, image, 52) << endl;
    return 0;
}