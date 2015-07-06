""" Nose Tests for OCLS
"""

import ocls_framework as ocls
import numpy as np
from nose.tools import *

class TestFramework:
    """ Test framework methods
    """
    def setup(self):
        self.ctx = ocls.Framework()

    def teardown(self):
        import time
        self.ctx = None

    @classmethod
    def setup_class(cls):
        pass

    @classmethod
    def teardown_class(cls):
        pass


class TestData:
    """ Test data methods
    """
    def setup(self):
        self.ctx = ocls.Framework()

    def teardown(self):
        import time
        self.ctx = None

    @classmethod
    def setup_class(cls):
        pass

    @classmethod
    def teardown_class(cls):
        pass

    def test_create_empty_1D(self):
        """ Test creating an zero initialized 1D dataset
        """
        d = ocls.Domain(1)
        d.setMesh(x=4)
        data = self.ctx.createData(d,"")

        # Every cell should be zero initialized
        for i in range(4):
            assert_equals(data[i], 0.0)

    def test_create_empty_1D_numpy(self):
        """ Test creating an zero initialized 1D dataset (numpy)
        """
        d = ocls.Domain(1)
        d.setMesh(x=4)
        data = self.ctx.createData(d,"")

        numpy_array = data.toArray()
        zero_array = np.zeros(4)

        # Length should be equal to mesh size
        assert_equals(len(numpy_array), 4)

        # Every cell should be zero initialized
        for i in range(4):
            assert_equals(numpy_array[i], zero_array[i])

    def test_create_empty_2D(self):
        """ Test creating an zero initialized 2D dataset
        """
        d = ocls.Domain(2)
        d.setMesh(x=4,y=4)
        data = self.ctx.createData(d,"")

        # Every cell should be zero initialized
        for i in range(4):
            for j in range(4):
                assert_equals(data[i,j], 0.0)

    def test_create_empty_2D_numpy(self):
        """ Test creating an zero initialized 2D dataset  (numpy)
        """
        d = ocls.Domain(2)
        d.setMesh(x=4,y=4)
        data = self.ctx.createData(d,"")

        numpy_array = data.toArray()
        zero_array = np.zeros((4,4))

        # Length should be equal to mesh size
        assert_equals(len(numpy_array), 4)
        assert_equals(len(numpy_array[0]), 4)

        # Every cell should be zero initialized
        for i in range(4):
            for j in range(4):
                assert_equals(numpy_array[i][j], zero_array[i][j])

    def test_create_empty_3D(self):
        """ Test creating an zero initialized 3D dataset
        """
        d = ocls.Domain(3)
        d.setMesh(x=4,y=4,z=4)
        data = self.ctx.createData(d,"")

        # Every cell should be zero initialized
        for i in range(4):
            for j in range(4):
                for k in range(4):
                    assert_equals(data[i,j,k], 0.0)

    def test_create_empty_3D_numpy(self):
        """ Test creating an zero initialized 3D dataset  (numpy)
        """
        d = ocls.Domain(3)
        d.setMesh(x=4,y=4,z=4)
        data = self.ctx.createData(d,"")

        numpy_array = data.toArray()
        zero_array = np.zeros((4,4,4))

        # Length should be equal to mesh size
        assert_equals(len(numpy_array), 4)
        assert_equals(len(numpy_array[0]), 4)
        assert_equals(len(numpy_array[0][0]), 4)

        # Every cell should be zero initialized
        for i in range(4):
            for j in range(4):
                for k in range(4):
                    assert_equals(numpy_array[i][j][k], zero_array[i][j][k])

    def test_set_data_lambda_1D(self):
        """ Test setting 1D data with python lambda
        """
        d = ocls.Domain(1)
        d.setMesh(x=4)
        data = self.ctx.createData(d,"")


        def I(p):
            return p[0]
        data.set(I)

        for i in range(4):
            assert_almost_equal(data[i], d.toLinspace()[i],places=6)

    def test_set_data_lambda_2D(self):
        """ Test setting 2D data with python lambda
        """
        d = ocls.Domain(2)
        d.setMesh(x=4,y=4)
        data = self.ctx.createData(d,"")


        def I(p):
            return p[0]+p[1]
        data.set(I)

        for i in range(4):
            for j in range(4):
                assert_almost_equal(data[i,j], d.toLinspace()[0][i]+d.toLinspace()[1][j],places=6)

    def test_set_data_lambda_3D(self):
        """ Test setting 3D data with python lambda
        """
        d = ocls.Domain(3)
        d.setMesh(x=4,y=4,z=4)
        data = self.ctx.createData(d,"")


        def I(p):
            return p[0]+p[1]+p[2]
        data.set(I)

        for i in range(4):
            for j in range(4):
                for k in range(4):
                    assert_almost_equal(data[i,j,k], d.toLinspace()[0][i]+d.toLinspace()[1][j]+d.toLinspace()[2][k],places=6)


    def test_non_uniform_grid_2D(self):
        """ Test creating a non-uniform grid 2D with numpy array
        """
        Nx = 4
        Ny = 2

        d = ocls.Domain(2)
        d.setMesh(x=Nx,y=Ny)
        data = self.ctx.createData(d,"")

        to_set = np.array([
            [1.0,2.0,3.0,4.0],
            [1.0,2.0,3.0,4.0]
        ]).astype('float32')
        data.set(to_set)

        for i in range(Nx):
            for j in range(Ny):
                assert_almost_equal(data[i,j], to_set[j,i], places=6)

        for i in range(Nx):
            for j in range(Ny):
                assert_almost_equal(data[i,j], data.toArray()[j,i], places=6)

    def test_non_uniform_grid_lambda_2D(self):
        """ Test creating a non-uniform grid 2D with lambda
        """
        Nx = 4
        Ny = 2

        d = ocls.Domain(2)
        d.setMesh(x=Nx,y=Ny)
        data = self.ctx.createData(d,"")

        def I(p):
            return p[0]
        data.set(I)

        for i in range(Nx):
            print "i", i
            assert_almost_equal(data[i,0], d.toLinspace()[0][i], places=6)

        def I(p):
            return p[1]
        data.set(I)

        for i in range(Ny):
            print "i", i
            assert_almost_equal(data[0,i], d.toLinspace()[1][i], places=6)

    def test_non_uniform_grid_3D(self):
        """ Test creating a non-uniform grid 3D with numpy array
        """
        Nx = 4
        Ny = 3
        Nz = 2

        d = ocls.Domain(3)
        d.setMesh(x=Nx,y=Ny,z=Nz)
        data = self.ctx.createData(d,"")

        to_set = np.array([
            [[1.0,2.0,3.0,4.0],
            [1.0,2.0,3.0,4.0],
            [1.0,2.0,3.0,4.0]],

            [[1.0,2.0,3.0,4.0],
            [1.0,2.0,3.0,4.0],
            [1.0,2.0,3.0,4.0]]
        ]).astype('float32')
        data.set(to_set)

        for i in range(Nx):
            for j in range(Ny):
                for k in range(Nz):
                    assert_almost_equal(data[i,j,k], to_set[k,j,i], places=6)

        for i in range(Nx):
            for j in range(Ny):
                for k in range(Nz):
                    assert_almost_equal(data[i,j,k], data.toArray()[k,j,i], places=6)

    def test_non_uniform_grid_lambda_3D(self):
        """ Test creating a non-uniform grid 3D with lambda
        """
        Nx = 4
        Ny = 3
        Nz = 2

        d = ocls.Domain(3)
        d.setMesh(x=Nx,y=Ny,z=Nz)
        data = self.ctx.createData(d,"")

        def I(p):
            return p[0]
        data.set(I)

        for i in range(Nx):
            assert_almost_equal(data[i,0,0], d.toLinspace()[0][i], places=6)

        def I(p):
            return p[1]
        data.set(I)

        for i in range(Ny):
            assert_almost_equal(data[0,i,0], d.toLinspace()[1][i], places=6)

        def I(p):
            return p[2]
        data.set(I)

        for i in range(Nz):
            assert_almost_equal(data[0,0,i], d.toLinspace()[2][i], places=6)

    def test_set_data_1D(self):
        """ Test setting 1D data with numpy np.array()
        """
        d = ocls.Domain(1)
        d.setMesh(x=4)
        data = self.ctx.createData(d,"")

        to_set = np.array([1.0,2.0,3.0,4.0]).astype('float32')
        data.set(to_set)

        for i in range(4):
            assert_equals(data[i], to_set[i])

    def test_set_data_2D(self):
        """ Test setting 2D data with numpy np.array()
        """
        d = ocls.Domain(2)
        d.setMesh(x=4,y=4)
        data = self.ctx.createData(d,"")

        to_set = np.array([
            [1.0,2.0,3.0,4.0],
            [1.0,2.0,3.0,4.0],
            [1.0,2.0,3.0,4.0],
            [1.0,2.0,3.0,4.0]
        ]).astype('float32')
        data.set(to_set)

        for i in range(4):
            for j in range(4):
                assert_equals(data[j,i], to_set[i][j])

    def test_set_data_3D(self):
        """ Test setting 3D data with numpy np.array()
        """
        d = ocls.Domain(3)
        d.setMesh(x=4,y=4,z=4)
        data = self.ctx.createData(d,"")

        to_set = np.array([
            [[1.0,2.0,3.0,4.0],
            [1.0,2.0,3.0,4.0],
            [1.0,2.0,3.0,4.0],
            [1.0,2.0,3.0,4.0]],

            [[1.0,2.0,3.0,4.0],
            [1.0,2.0,3.0,4.0],
            [1.0,2.0,3.0,4.0],
            [1.0,2.0,3.0,4.0]],

            [[1.0,2.0,3.0,4.0],
            [1.0,2.0,3.0,4.0],
            [1.0,2.0,3.0,4.0],
            [1.0,2.0,3.0,4.0]],

            [[1.0,2.0,3.0,4.0],
            [1.0,2.0,3.0,4.0],
            [1.0,2.0,3.0,4.0],
            [1.0,2.0,3.0,4.0]]
        ]).astype('float32')
        data.set(to_set)

        for i in range(4):
            for j in range(4):
                for k in range(4):
                        assert_equals(data[k,j,i], to_set[i][j][k])

    def test_find_max_1D(self):
        """ Test finding maximum value in 1D dataset
        """
        d = ocls.Domain(1)
        d.setMesh(x=500)
        data = self.ctx.createData(d,"")

        to_set = np.zeros(500).astype('float32')
        to_set[250] = 1.0
        data.set(to_set)

        assert_equals(data.max(), 1.0)

    def test_find_max_2D(self):
        """ Test finding maximum value in 2D dataset
        """
        d = ocls.Domain(2)
        d.setMesh(x=500,y=500)
        data = self.ctx.createData(d,"")

        to_set = np.zeros((500,500)).astype('float32')
        to_set[250,250] = 1.0
        data.set(to_set)

        assert_equals(data.max(), 1.0)

    def test_find_max_3D(self):
        """ Test finding maximum value in 3D dataset
        """
        d = ocls.Domain(3)
        d.setMesh(x=100,y=100,z=100)
        data = self.ctx.createData(d,"")

        to_set = np.zeros((100,100,100)).astype('float32')
        to_set[50,50,50] = 1.0
        data.set(to_set)

        assert_equals(data.max(), 1.0)

    def test_find_min_1D(self):
        """ Test finding minimum value in 1D dataset
        """
        d = ocls.Domain(1)
        d.setMesh(x=500)
        data = self.ctx.createData(d,"")

        to_set = np.zeros(500).astype('float32')
        to_set[250] = -1.0
        data.set(to_set)

        assert_equals(data.min(), -1.0)

    def test_find_min_2D(self):
        """ Test finding minimum value in 2D dataset
        """
        d = ocls.Domain(2)
        d.setMesh(x=500,y=500)
        data = self.ctx.createData(d,"")

        to_set = np.zeros((500,500)).astype('float32')
        to_set[250,250] = -1.0
        data.set(to_set)

        assert_equals(data.min(), -1.0)

    def test_find_min_3D(self):
        """ Test finding minimum value in 3D dataset
        """
        d = ocls.Domain(3)
        d.setMesh(x=100,y=100,z=100)
        data = self.ctx.createData(d,"")

        to_set = np.zeros((100,100,100)).astype('float32')
        to_set[50,50,50] = -1.0
        data.set(to_set)

        assert_equals(data.min(), -1.0)

    def test_create_large_1D(self):
        """ Test creating large dataset 1D
            sizes = [10000,100000,500000,10000000]
        """
        sizes = [10000,100000,500000,10000000]
        for i in range(len(sizes)):
            yield self.create_data, sizes[i], 1

    def test_create_large_2D(self):
        """ Test creating large dataset 2D
            sizes = [500,1000,2000,5000]
        """
        sizes = [500,1000,2000,5000]
        for i in range(len(sizes)):
            yield self.create_data, sizes[i], 2

    def test_create_large_3D(self):
        """ Test creating large dataset 3D
            sizes = [25,50,100,500]
        """
        sizes = [25,50,100,500]
        for i in range(len(sizes)):
            yield self.create_data, sizes[i], 3

    def create_data(self, N, d):
        """ N
        """
        d = ocls.Domain(d)
        d.setMesh(x=N,y=N,z=N)
        data = self.ctx.createData(d,"")

    def test_copy_data_1D(self):
        """ Test copying data 1D
        """
        d = ocls.Domain(1)
        d.setMesh(x=4)

        this = self.ctx.createData(d,"")
        other = self.ctx.createData(d,"")

        to_set = np.array([1.0,2.0,3.0,4.0]).astype('float32')
        this.set(to_set)

        other.copy(this)

        for i in range(4):
            assert_equals(this[i], other[i])

    def test_copy_data_2D(self):
        """ Test copying data 2D
        """
        d = ocls.Domain(2)
        d.setMesh(x=4,y=4)

        this = self.ctx.createData(d,"")
        other = self.ctx.createData(d,"")

        to_set = np.array([
            [1.0,2.0,3.0,4.0],
            [1.0,2.0,3.0,4.0],
            [1.0,2.0,3.0,4.0],
            [1.0,2.0,3.0,4.0]
        ]).astype('float32')
        this.set(to_set)

        other.copy(this)

        for i in range(4):
            assert_equals(this[i], other[i])

    def test_copy_data_3D(self):
        """ Test copying data 3D
        """
        d = ocls.Domain(3)
        d.setMesh(x=4,y=4,z=4)

        this = self.ctx.createData(d,"")
        other = self.ctx.createData(d,"")

        to_set = np.array([
            [[1.0,2.0,3.0,4.0],
            [1.0,2.0,3.0,4.0],
            [1.0,2.0,3.0,4.0],
            [1.0,2.0,3.0,4.0]],

            [[1.0,2.0,3.0,4.0],
            [1.0,2.0,3.0,4.0],
            [1.0,2.0,3.0,4.0],
            [1.0,2.0,3.0,4.0]],

            [[1.0,2.0,3.0,4.0],
            [1.0,2.0,3.0,4.0],
            [1.0,2.0,3.0,4.0],
            [1.0,2.0,3.0,4.0]],

            [[1.0,2.0,3.0,4.0],
            [1.0,2.0,3.0,4.0],
            [1.0,2.0,3.0,4.0],
            [1.0,2.0,3.0,4.0]]
        ]).astype('float32')
        this.set(to_set)

        other.copy(this)

        for i in range(4):
            assert_equals(this[i], other[i])

class TestCallableFunction:
    """ Test callable function methods
    """
    def setup(self):
        self.ctx = ocls.Framework()

    def teardown(self):
        import time
        self.ctx = None

    @classmethod
    def setup_class(cls):
        pass

    @classmethod
    def teardown_class(cls):
        pass
