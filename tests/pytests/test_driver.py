#!/usr/bin/env python
# ======================================================================
#
#                           Brad T. Aagaard
#                        U.S. Geological Survey
#
# ======================================================================
#

import unittest

class TestApp(object):
  """
  Test application.
  """

  def __init__(self):
    """
    Constructor.
    """
    return

  def main(self):
    """Run the application.
    """
    success = unittest.TextTestRunner(verbosity=2).run(self._suite()).wasSuccessful()

    if not success:
      import sys
      sys.exit(1)
    return
  

  def _suite(self):
    """
    Setup the test suite.
    """

    suite = unittest.TestSuite()

    from TestGeneratePoints import TestGeneratePointsBlock1
    suite.addTest(unittest.makeSuite(TestGeneratePointsBlock1))

    from TestGeneratePoints import TestGeneratePointsBlock2
    suite.addTest(unittest.makeSuite(TestGeneratePointsBlock2))

    from TestGeneratePoints import TestGeneratePointsBlockFlat
    suite.addTest(unittest.makeSuite(TestGeneratePointsBlockFlat))

    return suite


# ----------------------------------------------------------------------
if __name__ == '__main__':
  app = TestApp()
  app.main()


# End of file 
