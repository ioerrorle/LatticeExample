#include <QtTest>
#include <src/geometry/kdTree/KDTree.h>
#include <src/lattice/LatticeDeform.h>

class LatticeAlgoTest : public QObject
{
    Q_OBJECT

public:
    LatticeAlgoTest();
    ~LatticeAlgoTest();

private:
   const QList<int> enumVertices(QList<QVector3D> &vertices);
private slots:
    void test_points_split();
    void test_kd_tree();
    void test_empty_tree();
    void test_kd_tree_with_zeros();
    void test_find_closest_in_radius();
    void testCalcTransformWeights();

};

const QList<int> LatticeAlgoTest::enumVertices(QList<QVector3D> &vertices) {
    QList<int> result;
    for (int i = 0; i < vertices.size(); i++) {
        result << i;
    }
    return result;
}

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
   KDTree::splitPoints(indices, vertices.toVector(), 0, 0, left, right);
   QVERIFY(left.size() == 0 && right.size() == 0 && left.size() == 0 && right.size() == 0);

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

void LatticeAlgoTest::test_empty_tree() {
    QList<int> indices;
    QList<QVector3D> vertices;
    int numOfLeaves = 0;
    KDTree::buildTree(indices, vertices.toVector(), 1, &numOfLeaves);
    QVERIFY(numOfLeaves == 1);
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
   float radius = 2;

   float radiusSquared = pow(radius, 2);
   node->findVerticesInRadius(QVector3D(0,0,0), radiusSquared, newIndices, newPoints);
   QVERIFY(newIndices.size() == 4);
   QVERIFY(newPoints.size() == 4);

   newIndices.clear();
   newPoints.clear();
   radius = 0.5;
   radiusSquared = pow(radius, 2);
   node->findVerticesInRadius(QVector3D(0,0,0), radiusSquared, newIndices, newPoints);
   QVERIFY(newPoints.size() == 1);
   QVERIFY(newIndices.size() == 1);

   newIndices.clear();
   newPoints.clear();
   node->findVerticesInRadius(QVector3D(10, 10, 10), radiusSquared, newIndices, newPoints);
   QVERIFY(newPoints.size() == 0);
   QVERIFY(newIndices.size() == 0);

   vertices.clear();
   indices.clear();
   newIndices.clear();
   newPoints.clear();
   node = KDTree::buildTree(indices, vertices.toVector(), 1);
   node->findVerticesInRadius(QVector3D(10, 10, 10), radiusSquared, newIndices, newPoints);
   QVERIFY(newPoints.size() == 0);
   QVERIFY(newIndices.size() == 0);
}

void LatticeAlgoTest::testCalcTransformWeights() {
    QList<QVector3D> restBody;
    QList<QVector3D> deformedBody;
    QList<QVector3D> bodyToTransform;
    QVector<QVector3D> output;

    bool result;

    result = LatticeDeform::deformVertices(restBody.toVector(), KDTree::buildTree(enumVertices(restBody), restBody.toVector(), 10), deformedBody.toVector(), bodyToTransform.toVector(), output, 0);
    QVERIFY(result);

    restBody << QVector3D(0,0,0);
    result = LatticeDeform::deformVertices(restBody.toVector(), KDTree::buildTree(enumVertices(restBody), restBody.toVector(), 10), deformedBody.toVector(), bodyToTransform.toVector(), output, 0);
    QVERIFY(!result);

    deformedBody << QVector3D(0,0,0);
    result = LatticeDeform::deformVertices(restBody.toVector(), KDTree::buildTree(enumVertices(restBody), restBody.toVector(), 10), deformedBody.toVector(), bodyToTransform.toVector(), output, 0);
    QVERIFY(result);

    bodyToTransform << QVector3D(0,0,0);
    result = LatticeDeform::deformVertices(restBody.toVector(), KDTree::buildTree(enumVertices(restBody), restBody.toVector(), 10), deformedBody.toVector(), bodyToTransform.toVector(), output, 0);
    QVERIFY(!result);

    output.append(QVector3D(0,0,0));
    result = LatticeDeform::deformVertices(restBody.toVector(), KDTree::buildTree(enumVertices(restBody), restBody.toVector(), 10), deformedBody.toVector(), bodyToTransform.toVector(), output, 0);
    QVERIFY(result);

    restBody.clear();
    deformedBody.clear();
    bodyToTransform.clear();
    output.clear();

    result = LatticeDeform::deformVertices(restBody.toVector(), KDTree::buildTree(enumVertices(restBody), restBody.toVector(), 10), deformedBody.toVector(), bodyToTransform.toVector(), output, 1);
    QVERIFY(result);

    restBody << QVector3D(0,0,0);
    result = LatticeDeform::deformVertices(restBody.toVector(), KDTree::buildTree(enumVertices(restBody), restBody.toVector(), 10), deformedBody.toVector(), bodyToTransform.toVector(), output, 1);
    QVERIFY(!result);

    deformedBody << QVector3D(0,0,0);
    result = LatticeDeform::deformVertices(restBody.toVector(), KDTree::buildTree(enumVertices(restBody), restBody.toVector(), 10), deformedBody.toVector(), bodyToTransform.toVector(), output, 1);
    QVERIFY(result);

    bodyToTransform << QVector3D(0,0,0);
    result = LatticeDeform::deformVertices(restBody.toVector(), KDTree::buildTree(enumVertices(restBody), restBody.toVector(), 10), deformedBody.toVector(), bodyToTransform.toVector(), output, 1);
    QVERIFY(!result);

    output.append(QVector3D(0,0,0));
    result = LatticeDeform::deformVertices(restBody.toVector(), KDTree::buildTree(enumVertices(restBody), restBody.toVector(), 10), deformedBody.toVector(), bodyToTransform.toVector(), output, 1);
    QVERIFY(result);
}
QTEST_APPLESS_MAIN(LatticeAlgoTest)

#include "tst_latticealgotest.moc"
