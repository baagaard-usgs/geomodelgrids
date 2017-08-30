# ======================================================================
#
#                           Brad T. Aagaard
#                        U.S. Geological Survey
#
# ======================================================================
#

import unittest
import numpy
import sys

sys.path.append("../../src/scripts")
from generate_points import App

class TestGeneratePoints(unittest.TestCase):

    def setUp(self):
        """Setup test data.
        """
        self.data_dir = "data"
        self.dataE_dir = "dataE"

        self.model_config = None
        self.blocksE = None
        return


    def test_write_surfxy(self):
        """Test writing points on ground surface for topography.
        """
        app = App(self.model_config, self.data_dir)
        app.run("groundsurf")
        
        groundsurf_filename = "%s/%s-topo-xy.txt.gz" % (self.data_dir, app.model.key)
        groundsurf = numpy.loadtxt(groundsurf_filename)

        groundsurfE_filename = "%s/%s-topo-xy.txt.gz" % (self.dataE_dir, app.model.key)
        groundsurfE = numpy.loadtxt(groundsurfE_filename)

        self._check(groundsurfE, groundsurf)
        return

    def test_write_blocks(self):
        """Test writing points in blocks.
        """
        app = App(self.model_config, self.data_dir)
        app.run("blocks")

        for block_name in self.blocksE:
            block_filename = "%s/%s-%s-xyz.txt.gz" % (self.data_dir, app.model.key, block_name)
            blockE_filename = "%s/%s-%s-xyz.txt.gz" % (self.dataE_dir, app.model.key, block_name)

            block = numpy.loadtxt(block_filename)
            blockE = numpy.loadtxt(blockE_filename)

            self._check(blockE, block)
        return

    def _check(self, valuesE, values):
        """Verify arrays match.
        """
        shapeE = valuesE.shape
        shape = values.shape
        self.assertEqual(len(shapeE), len(shape))
        for vE,v in zip(shapeE,shape):
            self.assertEqual(vE, v)

        tolerance = 1.0e-6
        okay = numpy.zeros(valuesE.shape, dtype=numpy.bool)

        maskRatio = valuesE > tolerance
        ratio = numpy.abs(1.0 - values[maskRatio] / valuesE[maskRatio])
        if len(ratio) > 0:
            okay[maskRatio] = ratio < tolerance
        
        maskDiff = ~maskRatio
        diff = numpy.abs(valuesE[maskDiff] - values[maskDiff])
        if len(diff) > 0:
            okay[maskDiff] = diff < tolerance

        if numpy.prod(shapeE) != numpy.sum(okay):
            print("Expected values (not okay): %s" % valuesE[okay])
            print("Computed values (not okay): %s" % values[okay])
        self.assertEqual(numpy.prod(shapeE), numpy.sum(okay))
        return
  
class TestGeneratePointsBlock1(TestGeneratePoints):
    """
    Test generate_points for model with 1 layer.
    """

    def setUp(self):
        """Set up test data.
        """
        TestGeneratePoints.setUp(self)
        self.model_config = "dataE/points_one.cfg"
        self.blocksE = ["block0"]
        return
    

class TestGeneratePointsBlock2(TestGeneratePoints):
    """
    Test generate_points for model with 2 layers.
    """

    def setUp(self):
        """Set up test data.
        """
        TestGeneratePoints.setUp(self)
        self.model_config = "dataE/points_two.cfg"
        self.blocksE = ["block0", "block1"]
        return
    
class TestGeneratePointsBlockFlat(TestGeneratePoints):
    """
    Test generate_points for model with 1 block and no topography.
    """

    def setUp(self):
        """Set up test data.
        """
        TestGeneratePoints.setUp(self)
        self.model_config = "dataE/points_flat.cfg"
        self.blocksE = ["block0"]
        return
    
# End of file 
