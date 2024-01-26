"""Test _geomodelgrids.Model.
"""

import unittest

import geomodelgrids


class TestModelInfo(unittest.TestCase):

    FILENAME = "../data/one-block-flat.h5"

    INFO = {
        "title": "One Block Flat",
        "id": "one-block-flat",
        "description": "Model with one block and no topography.",
        "keywords": ["key one", "key two", "key three"],
        "history": "First version",
        "comment": "One comment",
        "creator_name": "John Doe",
        "creator_institution": "Agency",
        "creator_email": "johndoe@agency.org",
        "acknowledgement": "Thank you!",
        "authors": ["Smith, Jim", "Doe, John", "Doyle, Sarah"],
        "references": ["Reference 1", "Reference 2"],
        "repository_name": "Some repository",
        "repository_url": "http://somewhere.org",
        "repository_doi": "this.is.a.doi",
        "version": "1.0.0",
        "license": "CC0",
        "auxiliary": """{"0": "zero", "1": "one"}""",
    }

    def test_metadata(self):
        model = geomodelgrids.Model()
        model.open(self.FILENAME, model.READ)
        model.load_metadata()
        info = model.get_info()
        self.assertEqual(self.INFO["title"], info.title)
        self.assertEqual(self.INFO["id"], info.id)
        self.assertEqual(self.INFO["description"], info.description)
        self.assertEqual(self.INFO["keywords"], info.keywords)
        self.assertEqual(self.INFO["history"], info.history)
        self.assertEqual(self.INFO["comment"], info.comment)
        self.assertEqual(self.INFO["creator_name"], info.creator_name)
        self.assertEqual(self.INFO["creator_institution"], info.creator_institution)
        self.assertEqual(self.INFO["creator_email"], info.creator_email)
        self.assertEqual(self.INFO["acknowledgement"], info.acknowledgement)
        self.assertEqual(self.INFO["authors"], info.authors)
        self.assertEqual(self.INFO["references"], info.references)
        self.assertEqual(self.INFO["repository_name"], info.repository_name)
        self.assertEqual(self.INFO["repository_url"], info.repository_url)
        self.assertEqual(self.INFO["repository_doi"], info.repository_doi)
        self.assertEqual(self.INFO["version"], info.version)
        self.assertEqual(self.INFO["license"], info.license)
        self.assertEqual(self.INFO["auxiliary"], info.auxiliary)
        model.close()


def load_tests(loader, tests, pattern):
    TEST_CLASSES = [TestModelInfo]

    suite = unittest.TestSuite()
    for cls in TEST_CLASSES:
        suite.addTests(loader.loadTestsFromTestCase(cls))
    return suite


if __name__ == "__main__":
    unittest.main(verbosity=2)


# End of file
