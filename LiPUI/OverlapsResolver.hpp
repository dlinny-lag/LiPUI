#pragma once
#include <algorithm>
#include <vector>
#include "Helper3D.hpp"

namespace Overlaps
{
	struct Rectangle
	{
		UInt32 Id;
		SInt32 X;
		SInt32 Y;
		SInt32 Width;
		SInt32 Height;

		Point2D Center() const
		{
			return Point2D{X + Width/2, Y + Height/2};
		}
		bool Intersects(const Rectangle& other, Point2D& difference, SInt32& maxWidth, SInt32& maxHeight) const
		{
			const Point2D center = Center();
            const Point2D otherCenter = other.Center();

            difference.X = otherCenter.X - center.X;
            difference.Y = otherCenter.Y - center.Y;

            maxWidth = std::max(Width, other.Width);
            maxHeight = std::max(Height, other.Height);

            return std::abs(difference.X) < maxWidth && std::abs(difference.Y) < maxHeight;
		}
	};

	struct RectangleLess
	{
		bool operator()(const Rectangle& a, const Rectangle& b) const
		{
			if (a.X < b.X)
				return true;
			if (a.X > b.X)
				return false;
			return a.Y < b.Y;
		}

	};

    template <typename I> std::string ToHexString(I w, size_t hex_len = sizeof(I)<<1)
	{
	    static const char* digits = "0123456789ABCDEF";
	    std::string rc(hex_len,'0');
	    for (size_t i=0, j=(hex_len-1)*4 ; i<hex_len; ++i,j-=4)
	        rc[i] = digits[(w>>j) & 0x0f];
	    return rc;
	}

	struct Resolver
	{
		static void Resolve(std::vector<Rectangle>& rects)
		{
            if (rects.size() < 2)
                return;

            //std::string report = "Trying to resolve:\n";
            //for (const auto& rect : rects)
            //{
	           // report += ToHexString(rect.Id) + ": {" + std::to_string(rect.X) + ", " + std::to_string(rect.Y) + "}. W=" + std::to_string(rect.Width) + ", H=" +std::to_string(rect.Height) + "\n";
            //}
            //_MESSAGE("%s", report.c_str());

			bool changed;
			do
            {
				std::sort(rects.begin(), rects.end(), RectangleLess());
                changed = false;
                for (size_t i = 0; i < rects.size()-1; i++)
                {
                    changed |= ResolvePair(rects[i], rects[i + 1]);
                }

            } while (changed);
		}

		static bool ResolvePair(Rectangle& r1, Rectangle& r2)
		{
			Point2D difference;
			SInt32 maxWidth, maxHeight;
			if (!r1.Intersects(r2, difference, maxWidth, maxHeight))
                return false; // nothing to resolve

            const Point2D r1Center = r1.Center();
            const Point2D r2Center = r2.Center();
            const SInt32 absDiffX = std::abs(difference.X);
            const SInt32 absDiffY = std::abs(difference.Y);
			if (absDiffX < absDiffY)
            {
                const SInt32 diffY = r1Center.Y - r2Center.Y;
                if (diffY > 0)
                { // TODO: handle both heights
                    int shiftY = (maxHeight - diffY)/2;
                    if (shiftY == 0)
                        shiftY++; // prevent 0 change
                    r1.Y += shiftY;
                    r2.Y -= shiftY;
                    return true;
                }

                if (diffY <= 0) // always true
                {// TODO: handle both heights
                    int shiftY = (maxHeight + diffY)/2;
                    if (shiftY == 0)
                        shiftY++; // prevent 0 change
                    r1.Y -= shiftY;
                    r2.Y += shiftY;
                    return true;
                }
            }

            if (absDiffY <= absDiffX) // always true
            {
                const int diffX = r1Center.X - r2Center.X;
                if (diffX <= 0)
                { // TODO: handle both widths
                    int shiftX = (maxWidth + diffX)/2;
                    if (shiftX == 0)
                        shiftX++; // prevent 0 change
                    r1.X -= shiftX;
                    r2.X += shiftX;
                    return true;
                }

                if (diffX > 0) // always true
                { // TODO: handle both widths
                    int shiftX = (maxWidth - diffX)/2;
                    if (shiftX == 0)
                        shiftX++; // prevent 0 change
                    r1.X += shiftX;
                    r2.X -= shiftX;
                    return true;
                }

                return true;
            }
            throw "Invalid condition";
		}
	};

}