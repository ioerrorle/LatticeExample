#include <QtTest>
#include <src/geometry/kdTree/KDTree.h>

class LatticeAlgoTest : public QObject
{
    Q_OBJECT

public:
    LatticeAlgoTest();
    ~LatticeAlgoTest();

private slots:
    void test_points_split();
    void test_find_closest_in_radius();

};

LatticeAlgoTest::LatticeAlgoTest()
{

}

LatticeAlgoTest::~LatticeAlgoTest()
{

}

void LatticeAlgoTest::test_points_split()
{
   QList<int> indices;
   indices << 0 << 1 << 2 << 3;
   QList<QVector3D> vertices;
   vertices << QVector3D(0,0,0) << QVector3D(1,0,0) << QVector3D(0,1,0) << QVector3D(1,1,0);
   QList<int> left;
   QList<int> right;
   KDTree::splitPoints(indices, vertices.toVector(), 0, 0.5, left, right);
   QVERIFY(left.size() == 2);
   QVERIFY(right.size() == 2);
}

void LatticeAlgoTest::test_find_closest_in_radius()
{
   QList<int> indices;
   indices << 0 << 1 << 2 << 3;
   QList<QVector3D> vertices;
   vertices << QVector3D(0,0,0) << QVector3D(1,0,0) << QVector3D(0,1,0) << QVector3D(1,1,0);
   Node* node = KDTree::buildTree(indices, vertices.toVector(), 1);
   QVector<int> newIndices;
   QVector<QVector3D> newPoints;
   node->findVerticesInRadius(QVector3D(0,0,0), 4, newIndices, newPoints);
   QVERIFY(newIndices.size() == 4);
   QVERIFY(newPoints.size() == 4);
}
QTEST_APPLESS_MAIN(LatticeAlgoTest)

#include "tst_latticealgotest.moc"
