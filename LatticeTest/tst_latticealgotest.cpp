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
    void test_kd_tree();
    void test_kd_tree_with_zeros();
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

   vertices.clear();
   left.clear();
   right.clear();
   vertices << QVector3D(0,0,0) << QVector3D(0,0,0) << QVector3D(0,0,0) << QVector3D(0,0,0);
   KDTree::splitPoints(indices, vertices.toVector(), 0, 0, left, right);
   QVERIFY(left.size() == 0 || right.size() == 0);

   indices.clear();
   vertices.clear();
   left.clear();
   right.clear();
   indices << 0;
   vertices << QVector3D(0,0,0);
   KDTree::splitPoints(indices, vertices.toVector(), 0, 0, left, right);
   QVERIFY(left.size() == 0 || right.size() == 0);
}

void LatticeAlgoTest::test_kd_tree_with_zeros() {
    QList<int> indices;
    indices << 0 << 1 << 2 << 3;
    QList<QVector3D> vertices;
    vertices << QVector3D(0,0,0) << QVector3D(0,0,0) << QVector3D(0,0,0) << QVector3D(0,0,0);
    int numOfLeaves = 0;
    KDTree::buildTree(indices, vertices.toVector(), 1, &numOfLeaves);
    QVERIFY(numOfLeaves == 1);
}

void LatticeAlgoTest::test_kd_tree() {
    QList<int> indices;
    indices << 0 << 1 << 2 << 3;
    QList<QVector3D> vertices;
    vertices << QVector3D(0,0,0) << QVector3D(1,0,0) << QVector3D(0,1,0) << QVector3D(1,1,0);
    int numOfLeaves = 0;
    KDTree::buildTree(indices, vertices.toVector(), 1, &numOfLeaves);
    QVERIFY(numOfLeaves == 4);
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
