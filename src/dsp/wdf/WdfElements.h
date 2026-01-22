#pragma once

#include <cmath>

namespace wdf
{
    class OnePort
    {
    public:
        virtual ~OnePort() = default;
        virtual double getPortRes() const = 0;
        virtual void setIncident(double a) = 0;
        virtual double getReflected() const = 0;
        virtual double getVoltage() const = 0;
    };

    class Resistor final : public OnePort
    {
    public:
        void setResistance(double r)
        {
            R = r;
        }

        double getPortRes() const override { return R; }

        void setIncident(double aIn) override { a = aIn; }

        double getReflected() const override { return 0.0; }

        double getVoltage() const override { return 0.5 * a; }

    private:
        double R { 1000.0 };
        double a { 0.0 };
    };

    class Capacitor final : public OnePort
    {
    public:
        void prepare(double sampleRate, double capacitance)
        {
            fs = sampleRate;
            C = capacitance;
            portRes = 1.0 / (2.0 * C * fs);
        }

        double getPortRes() const override { return portRes; }

        void setIncident(double aIn) override { a = aIn; }

        double getReflected() const override
        {
            const double b = prevA;
            prevA = a;
            return b;
        }

        double getVoltage() const override { return 0.5 * (a + prevA); }

        void reset() { prevA = 0.0; }

    private:
        double fs { 44100.0 };
        double C { 1e-9 };
        double portRes { 1.0 };
        mutable double a { 0.0 };
        mutable double prevA { 0.0 };
    };

    class Inductor final : public OnePort
    {
    public:
        void prepare(double sampleRate, double inductance)
        {
            fs = sampleRate;
            L = inductance;
            portRes = 2.0 * L * fs;
        }

        double getPortRes() const override { return portRes; }

        void setIncident(double aIn) override { a = aIn; }

        double getReflected() const override
        {
            const double b = -prevA;
            prevA = a;
            return b;
        }

        double getVoltage() const override { return 0.5 * (a - prevA); }

        void reset() { prevA = 0.0; }

    private:
        double fs { 44100.0 };
        double L { 1.0 };
        double portRes { 1.0 };
        mutable double a { 0.0 };
        mutable double prevA { 0.0 };
    };

    class SeriesAdaptor final : public OnePort
    {
    public:
        SeriesAdaptor(OnePort& left, OnePort& right)
            : p1(left), p2(right)
        {
        }

        double getPortRes() const override
        {
            return p1.getPortRes() + p2.getPortRes();
        }

        void setIncident(double aIn) override
        {
            a = aIn;
            const double R1 = p1.getPortRes();
            const double R2 = p2.getPortRes();
            const double R = R1 + R2;

            const double b1 = p1.getReflected();
            const double b2 = p2.getReflected();

            const double a1 = a + (R1 / R) * (b2 - b1);
            const double a2 = a + (R2 / R) * (b1 - b2);

            p1.setIncident(a1);
            p2.setIncident(a2);

            cachedB = p1.getReflected() + p2.getReflected() - a;
        }

        double getReflected() const override { return cachedB; }

        double getVoltage() const override { return 0.5 * (a + cachedB); }

    private:
        OnePort& p1;
        OnePort& p2;
        double a { 0.0 };
        double cachedB { 0.0 };
    };

    class ParallelAdaptor final : public OnePort
    {
    public:
        ParallelAdaptor(OnePort& left, OnePort& right)
            : p1(left), p2(right)
        {
        }

        double getPortRes() const override
        {
            const double G1 = 1.0 / p1.getPortRes();
            const double G2 = 1.0 / p2.getPortRes();
            return 1.0 / (G1 + G2);
        }

        void setIncident(double aIn) override
        {
            a = aIn;
            const double G1 = 1.0 / p1.getPortRes();
            const double G2 = 1.0 / p2.getPortRes();
            const double G = G1 + G2;

            const double b1 = p1.getReflected();
            const double b2 = p2.getReflected();

            const double a1 = a + (G1 / G) * (b2 - b1);
            const double a2 = a + (G2 / G) * (b1 - b2);

            p1.setIncident(a1);
            p2.setIncident(a2);

            cachedB = p1.getReflected() + p2.getReflected() - a;
        }

        double getReflected() const override { return cachedB; }

        double getVoltage() const override { return 0.5 * (a + cachedB); }

    private:
        OnePort& p1;
        OnePort& p2;
        double a { 0.0 };
        double cachedB { 0.0 };
    };
}
