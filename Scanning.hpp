#pragma once
#include "LedSegment.hpp"

namespace Lights
{
	class Scanning
	{
	public:
		static uint Scan(LedSegment *segment);
		static uint ScanDual(LedSegment *segment);
		static uint ScanFull(LedSegment *segment);
		static uint ScanDualFull(LedSegment *segment);
		static uint ScanRandom(LedSegment *segment);
		static uint ScanDualRandom(LedSegment *segment);

	private:
		static uint ScanInternalRandom(LedSegment *segment, bool dual);
		static uint Scan(LedSegment *segment, Colour colour1, Colour colour2, bool dual, bool fullScan = false);
	};
}