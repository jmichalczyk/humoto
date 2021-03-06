/**
    @file
    @author  Alexander Sherikov
    @author  Jan Michalczyk
    @copyright 2014-2017 INRIA. Licensed under the Apache License, Version 2.0.
    (see @ref LICENSE or http://www.apache.org/licenses/LICENSE-2.0)

    @brief
*/


/**
 * @page EigenTools EigenTools
 *
 * @anchor eigentools_casting_hack
 *
 * @note Since an attempt to write to a temporary object may result in a
 * compilation error, it is not possible to use the result of matrix.block() as
 * an output parameter of a function. However, there is a workaround: the
 * parameter can be declared as const and then casted to non-const as suggested
 * in Eigen's documentation:
 * https://eigen.tuxfamily.org/dox/TopicFunctionTakingEigenTypes.html.
 * This hack is useful when it is necessary to handle matrix blocks and
 * matrices identically.
 */


#pragma once

/**
 * @defgroup EigenTools EigenTools
 * @brief Utility functions based on Eigen.
 */

/// @ingroup EigenTools
namespace etools
{
    typedef double  DefaultScalar;
    //#define EIGENTOOLS_ENABLE_EIGENTYPE_DETECTION


    typedef Eigen::Matrix<etools::DefaultScalar, Eigen::Dynamic, Eigen::Dynamic>   DefaultDynamicMatrix;


    /// @attention Won't work if 'Scalar' contains commas.
    #define EIGENTOOLS_DYNAMIC_MATRIX(Scalar)   Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic>

    /// @attention Won't work if 'Scalar' contains commas.
    #define EIGENTOOLS_DYNAMIC_VECTOR(Scalar)   Eigen::Matrix<Scalar, Eigen::Dynamic, 1>


    #ifdef EIGENTOOLS_DISABLE_ALIGNMENT_OF_STATIC_MATRICES
        #define EIGENTOOLS_CONSTANT_SIZE_ALIGN_TYPE   Eigen::DontAlign
    #else
        #define EIGENTOOLS_CONSTANT_SIZE_ALIGN_TYPE   Eigen::AutoAlign
    #endif

    #define  EIGENTOOLS_CONSTANT_SIZE_VECTOR(rows)          Eigen::Matrix<etools::DefaultScalar, rows, 1,    EIGENTOOLS_CONSTANT_SIZE_ALIGN_TYPE>
    #ifdef EIGEN_DEFAULT_TO_ROW_MAJOR
        // This may not work
        #define  EIGENTOOLS_CONSTANT_SIZE_MATRIX(rows, cols)    Eigen::Matrix<etools::DefaultScalar, rows, cols, EIGENTOOLS_CONSTANT_SIZE_ALIGN_TYPE>
    #else
        #define  EIGENTOOLS_CONSTANT_SIZE_MATRIX(rows, cols)    Eigen::Matrix<etools::DefaultScalar, rows, cols, EIGENTOOLS_CONSTANT_SIZE_ALIGN_TYPE | ((rows==1) ? (Eigen::RowMajor) : (Eigen::ColMajor))>
    #endif

    typedef EIGENTOOLS_CONSTANT_SIZE_VECTOR(2)  Vector2;
    typedef EIGENTOOLS_CONSTANT_SIZE_VECTOR(3)  Vector3;
    typedef EIGENTOOLS_CONSTANT_SIZE_VECTOR(4)  Vector4;
    typedef EIGENTOOLS_CONSTANT_SIZE_VECTOR(5)  Vector5;
    typedef EIGENTOOLS_CONSTANT_SIZE_VECTOR(6)  Vector6;
    typedef EIGENTOOLS_CONSTANT_SIZE_VECTOR(7)  Vector7;
    typedef EIGENTOOLS_CONSTANT_SIZE_VECTOR(8)  Vector8;
    typedef EIGENTOOLS_CONSTANT_SIZE_VECTOR(9)  Vector9;
    typedef EIGENTOOLS_CONSTANT_SIZE_VECTOR(10) Vector10;

    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(2, 2)  Matrix2;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(3, 3)  Matrix3;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(4, 4)  Matrix4;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(5, 5)  Matrix5;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(6, 6)  Matrix6;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(7, 7)  Matrix7;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(8, 8)  Matrix8;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(9, 9)  Matrix9;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(10, 10)  Matrix10;

    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(1, 1)  Matrix1x2;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(1, 3)  Matrix1x3;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(1, 4)  Matrix1x4;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(1, 5)  Matrix1x5;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(1, 6)  Matrix1x6;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(1, 7)  Matrix1x7;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(1, 8)  Matrix1x8;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(1, 9)  Matrix1x9;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(1, 10)  Matrix1x10;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(2, 3)  Matrix2x3;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(2, 4)  Matrix2x4;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(2, 5)  Matrix2x5;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(2, 6)  Matrix2x6;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(2, 7)  Matrix2x7;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(2, 8)  Matrix2x8;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(2, 9)  Matrix2x9;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(2, 10)  Matrix2x10;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(3, 2)  Matrix3x2;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(3, 4)  Matrix3x4;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(3, 5)  Matrix3x5;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(3, 6)  Matrix3x6;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(3, 7)  Matrix3x7;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(3, 8)  Matrix3x8;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(3, 9)  Matrix3x9;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(3, 10)  Matrix3x10;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(4, 2)  Matrix4x2;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(4, 3)  Matrix4x3;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(4, 5)  Matrix4x5;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(4, 6)  Matrix4x6;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(4, 7)  Matrix4x7;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(4, 8)  Matrix4x8;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(4, 9)  Matrix4x9;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(4, 10)  Matrix4x10;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(5, 2)  Matrix5x2;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(5, 3)  Matrix5x3;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(5, 4)  Matrix5x4;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(5, 6)  Matrix5x6;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(5, 7)  Matrix5x7;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(5, 8)  Matrix5x8;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(5, 9)  Matrix5x9;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(5, 10)  Matrix5x10;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(6, 2)  Matrix6x2;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(6, 3)  Matrix6x3;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(6, 4)  Matrix6x4;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(6, 5)  Matrix6x5;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(6, 7)  Matrix6x7;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(6, 8)  Matrix6x8;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(6, 9)  Matrix6x9;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(6, 10)  Matrix6x10;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(7, 2)  Matrix7x2;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(7, 3)  Matrix7x3;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(7, 4)  Matrix7x4;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(7, 5)  Matrix7x5;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(7, 6)  Matrix7x6;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(7, 8)  Matrix7x8;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(7, 9)  Matrix7x9;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(7, 10)  Matrix7x10;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(8, 2)  Matrix8x2;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(8, 3)  Matrix8x3;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(8, 4)  Matrix8x4;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(8, 5)  Matrix8x5;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(8, 6)  Matrix8x6;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(8, 7)  Matrix8x7;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(8, 9)  Matrix8x9;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(8, 10)  Matrix8x10;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(9, 2)  Matrix9x2;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(9, 3)  Matrix9x3;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(9, 4)  Matrix9x4;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(9, 5)  Matrix9x5;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(9, 6)  Matrix9x6;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(9, 7)  Matrix9x7;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(9, 8)  Matrix9x8;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(9, 10)  Matrix9x10;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(10, 2)  Matrix10x2;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(10, 3)  Matrix10x3;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(10, 4)  Matrix10x4;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(10, 5)  Matrix10x5;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(10, 6)  Matrix10x6;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(10, 7)  Matrix10x7;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(10, 8)  Matrix10x8;
    typedef EIGENTOOLS_CONSTANT_SIZE_MATRIX(10, 9)  Matrix10x9;



    /**
     * @brief Unset matrix (initialize to NaN)
     *
     * @tparam t_Derived matrix data type
     *
     * @param[in,out] matrix
     */
    template <typename t_Derived>
        void unsetMatrix (Eigen::DenseBase< t_Derived > &matrix)
    {
        if (std::numeric_limits<etools::DefaultScalar>::has_quiet_NaN)
        {
            matrix.setConstant(std::numeric_limits<etools::DefaultScalar>::quiet_NaN());
        }
        else
        {
            matrix.setZero();
        }
    }


    /**
     * @brief result = A^T * A
     *
     * @tparam t_DerivedOutput  Eigen parameter
     * @tparam t_DerivedInput   Eigen parameter
     *
     * @param[out] result
     * @param[in] A
     *
     * @attention Only the left lower triangular part of the result is initialized.
     */
    template<class t_DerivedOutput, class t_DerivedInput>
        void EIGENTOOLS_VISIBILITY_ATTRIBUTE
            getATA( Eigen::PlainObjectBase<t_DerivedOutput> &result,
                    const Eigen::DenseBase<t_DerivedInput> &A)
    {
        std::ptrdiff_t num_el = A.cols();
        result.resize(num_el,num_el);
        for (std::ptrdiff_t i = 0; i < num_el; ++i)
        {
            result.block(i, i, num_el-i, 1).noalias() = A.transpose().bottomRows(num_el-i) * A.col(i);
        }
    }


    /**
     * @brief result.diagonalBlock() = A^T * A
     *
     * @tparam t_DerivedOutput  Eigen parameter
     * @tparam t_DerivedInput   Eigen parameter
     *
     * @param[out] result
     * @param[in] A
     * @param[in] offset    offset of A in result
     * @param[in] num_el    size of result
     *
     * @attention Only the left lower triangular part of the result is initialized.
     */
    template<class t_DerivedOutput, class t_DerivedInput>
        void EIGENTOOLS_VISIBILITY_ATTRIBUTE
            getATA( Eigen::PlainObjectBase<t_DerivedOutput> &result,
                    const Eigen::DenseBase<t_DerivedInput> &A,
                    const std::ptrdiff_t offset,
                    const std::ptrdiff_t num_el)
    {
        std::ptrdiff_t A_num_col = A.cols();
        result.resize(num_el, num_el);
        result.template triangularView<Eigen::Lower>().setZero();
        for (std::ptrdiff_t i = 0; i < A_num_col; ++i)
        {
            result.block(   offset,
                            offset,
                            A_num_col,
                            A_num_col).block(i, i, A_num_col-i, 1).noalias() = A.transpose().bottomRows(A_num_col-i) * A.col(i);
        }
    }


    /**
     * @brief result += A^T * A
     *
     * @tparam t_DerivedOutput  Eigen parameter
     * @tparam t_DerivedInput   Eigen parameter
     *
     * @param[in,out] result
     * @param[in] A
     *
     * @attention Only the left lower triangular part of the result is initialized.
     */
    template<class t_DerivedOutput, class t_DerivedInput>
        void EIGENTOOLS_VISIBILITY_ATTRIBUTE
            addATA( Eigen::DenseBase<t_DerivedOutput> &result,
                    const Eigen::DenseBase<t_DerivedInput> &A)
    {
        std::ptrdiff_t num_el = A.cols();
        for (std::ptrdiff_t i = 0; i < num_el; ++i)
        {
            result.block(i, i, num_el-i, 1).noalias() += A.transpose().bottomRows(num_el-i) * A.col(i);
        }
    }


    /**
     * @brief result.diagonalBlock() += A^T * A
     *
     * @tparam t_DerivedOutput  Eigen parameter
     * @tparam t_DerivedInput   Eigen parameter
     *
     * @param[in,out] result
     * @param[in] A
     * @param[in] offset    offset of A in result
     *
     * @attention Only the left lower triangular part of the result is initialized.
     */
    template<class t_DerivedOutput, class t_DerivedInput>
        void EIGENTOOLS_VISIBILITY_ATTRIBUTE
            addATA( Eigen::DenseBase<t_DerivedOutput> &result,
                    const Eigen::DenseBase<t_DerivedInput> &A,
                    const std::ptrdiff_t offset)
    {
        std::ptrdiff_t A_num_col = A.cols();
        for (std::ptrdiff_t i = 0; i < A_num_col; ++i)
        {
            result.block(   offset,
                            offset,
                            A_num_col,
                            A_num_col).block(i, i, A_num_col-i, 1).noalias() += A.transpose().bottomRows(A_num_col-i) * A.col(i);
        }
    }



    /**
     * @brief Converts left lower triangular matrix to a symmetric matrix.
     *
     * @tparam t_Derived Eigen parameter
     *
     * @param[in,out] matrix
     */
    template<class t_Derived>
        void EIGENTOOLS_VISIBILITY_ATTRIBUTE
            convertLLTtoSymmetric(Eigen::PlainObjectBase<t_Derived> &matrix)
    {
        std::ptrdiff_t num_el = matrix.cols();
        for (std::ptrdiff_t i = 0; i < num_el-1; ++i)
        {
            matrix.block(i, i+1, 1, num_el-i-1) = matrix.transpose().block(i, i+1, 1, num_el-i-1);
        }
    }


    /**
     * @brief Transform the input positions given as a concatenated set of 2d/3d
     * vectors, given M = [v1, v2, ...], returns M_new = [R*v1 + t, R*v2 + t, ...]
     *
     * @tparam t_DerivedMatrix      Eigen parameter
     * @tparam t_DerivedRotation    Eigen parameter
     * @tparam t_DerivedTranslation Eigen parameter
     *
     * @param[in] matrix matrix containing vectors (M)
     * @param[in] rotation rotation matrix         (R)
     * @param[in] translation translation vector   (t)
     *
     * @return matrix of transformed vectors
     */
    template <class t_DerivedMatrix, class t_DerivedRotation, class t_DerivedTranslation>
        inline Eigen::Matrix<   typename Eigen::MatrixBase<t_DerivedMatrix>::Scalar,
                                Eigen::MatrixBase<t_DerivedMatrix>::RowsAtCompileTime,
                                Eigen::MatrixBase<t_DerivedMatrix>::ColsAtCompileTime>
        EIGENTOOLS_VISIBILITY_ATTRIBUTE
            transform(  const Eigen::MatrixBase<t_DerivedMatrix>        &matrix,
                        const Eigen::MatrixBase<t_DerivedRotation>      &rotation,
                        const Eigen::MatrixBase<t_DerivedTranslation>   &translation)
    {
        return ((rotation*matrix).colwise() + translation);
    }



    /**
     * @brief Create a diagonal matrix consisting of the input matrices
     *
     * @tparam t_Scalar         Eigen parameter (input)
     * @tparam t_rows           Eigen parameter (input)
     * @tparam t_cols           Eigen parameter (input)
     * @tparam t_flags          Eigen parameter (input)
     *
     * @param[in] input_matrices vector of smaller matrices to be placed into a large block diagonal matrix
     *
     * @return matrix with each input element as a block in the diagonal
     */
    template<   typename t_Scalar,
                int t_rows,
                int t_cols,
                int t_flags>
        EIGENTOOLS_DYNAMIC_MATRIX(t_Scalar)
        EIGENTOOLS_VISIBILITY_ATTRIBUTE
            makeBlockDiagonal(const std::vector<Eigen::Matrix<t_Scalar, t_rows, t_cols, t_flags> > &input_matrices)
    {
        switch (input_matrices.size())
        {
            case 0:
                return (EIGENTOOLS_DYNAMIC_MATRIX(t_Scalar)());

            case 1:
                return (input_matrices[0]);

            default:
                // Initialize the output diagonal matrix
                std::ptrdiff_t row_size = 0;
                std::ptrdiff_t col_size = 0;

                for(std::size_t i = 0; i < input_matrices.size(); ++i)
                {
                    row_size += input_matrices[i].rows();
                    col_size += input_matrices[i].cols();
                }
                EIGENTOOLS_DYNAMIC_MATRIX(t_Scalar) output;
                output.setZero(row_size, col_size);

                // Add in the input matrices
                std::ptrdiff_t cumulative_row = 0;
                std::ptrdiff_t cumulative_col = 0;

                for(std::size_t i = 0; i < input_matrices.size(); ++i)
                {
                    int current_num_rows = input_matrices[i].rows();
                    int current_num_cols = input_matrices[i].cols();

                    output.block(cumulative_row, cumulative_col, current_num_rows, current_num_cols) = input_matrices[i];

                    cumulative_row += current_num_rows;
                    cumulative_col += current_num_cols;
                }
                return output;
        }
    }



    /**
     * @brief Create a diagonal matrix replicating the input matrix
     *
     * @tparam t_DerivedInput   Eigen parameter (input)
     *
     * @param[in] input_matrix      matrix to be replicated into block diagonal
     * @param[in] num_copies        determines the output size
     *
     * @return matrix with each input element as a block in the diagonal
     */
    template<class t_DerivedInput>
        EIGENTOOLS_DYNAMIC_MATRIX(typename Eigen::DenseBase<t_DerivedInput>::Scalar)
        EIGENTOOLS_VISIBILITY_ATTRIBUTE
            makeBlockDiagonal(  const Eigen::DenseBase<t_DerivedInput> &input_matrix,
                                const std::ptrdiff_t num_copies)
    {
        switch (num_copies)
        {
            case 0:
                return (EIGENTOOLS_DYNAMIC_MATRIX(typename Eigen::DenseBase<t_DerivedInput>::Scalar)());

            case 1:
                return (input_matrix);

            default:
                // Initialize the output diagonal matrix
                EIGENTOOLS_DYNAMIC_MATRIX(typename Eigen::DenseBase<t_DerivedInput>::Scalar) output;

                output.setZero(num_copies * input_matrix.rows(),
                               num_copies * input_matrix.cols());

                // Replicate the input matrix into block diagonal form
                for(std::ptrdiff_t i = 0; i < num_copies; ++i)
                {
                    output.block(i*input_matrix.rows(), i*input_matrix.cols(),
                                 input_matrix.rows(), input_matrix.cols()) = input_matrix;
                }
                return (output);
        }
    }



    /**
     * @brief Concatenate matrices vertically, [A;B;C; ...]
     *
     * @tparam t_DerivedOutput  Eigen parameter
     * @tparam t_Scalar         Eigen parameter (input)
     * @tparam t_rows           Eigen parameter (input)
     * @tparam t_cols           Eigen parameter (input)
     * @tparam t_flags          Eigen parameter (input)
     *
     * @param[out] result    result of concatenation
     * @param[in] matrices  matrices
     */
    template<   class t_DerivedOutput,
                typename t_Scalar,
                int t_rows,
                int t_cols,
                int t_flags>
        void  EIGENTOOLS_VISIBILITY_ATTRIBUTE
            concatenateMatricesVertically(  Eigen::PlainObjectBase<t_DerivedOutput> &result,
                                            const std::vector<Eigen::Matrix<t_Scalar, t_rows, t_cols, t_flags> > &matrices)
    {
        std::vector< Eigen::Matrix<t_Scalar, t_rows, t_cols, t_flags> > nonempty_matrices  = matrices;
        std::size_t                  number_of_matrices = nonempty_matrices.size();

        if (number_of_matrices > 0)
        {
            std::ptrdiff_t total_number_of_rows = 0;
            std::ptrdiff_t number_of_cols = 0;

            for(std::size_t i = 0; i < number_of_matrices; ++i)
            {
                if(!nonempty_matrices[i].size())
                {
                    nonempty_matrices.erase(nonempty_matrices.begin() + i);
                    --number_of_matrices;
                }
            }

            if(!number_of_matrices)
            {
                result.resize(0, 0);
                return;
            }

            number_of_cols = nonempty_matrices[0].cols();
            for (std::size_t i = 0; i < number_of_matrices; ++i)
            {
                EIGENTOOLS_ASSERT(number_of_cols == nonempty_matrices[i].cols(), "Inconsistent size of input matrices.");
                total_number_of_rows += nonempty_matrices[i].rows();
            }

            result.resize(total_number_of_rows, number_of_cols);

            std::ptrdiff_t row_index = 0;
            for (std::size_t i = 0; i < number_of_matrices; ++i)
            {
                result.block(row_index, 0, nonempty_matrices[i].rows(), number_of_cols) = nonempty_matrices[i];
                row_index += nonempty_matrices[i].rows();
            }
        }
        else
        {
            result.resize(0, 0);
        }
    }


    /**
     * @brief Concatenate matrices horizontally, [A B C ...]
     *
     * @tparam t_DerivedOutput  Eigen parameter
     * @tparam t_DerivedInput1  Eigen parameter
     * @tparam t_DerivedInput2  Eigen parameter
     *
     * @param[out] result    result of concatenation
     * @param[in] matrix1
     * @param[in] matrix2
     */
    template<   class t_DerivedOutput,
                class t_DerivedInput1,
                class t_DerivedInput2>
        void  EIGENTOOLS_VISIBILITY_ATTRIBUTE
            concatenateMatricesHorizontally(
                Eigen::PlainObjectBase<t_DerivedOutput> &result,
                const Eigen::DenseBase<t_DerivedInput1> &matrix1,
                const Eigen::DenseBase<t_DerivedInput2> &matrix2)
    {
        if (matrix1.rows() == 0)
        {
            result = matrix2;
        }
        else
        {
            if (matrix2.rows() == 0)
            {
                result = matrix1;
            }
            else
            {
                std::ptrdiff_t  number_of_rows = matrix1.rows();
                std::ptrdiff_t  number_of_cols = matrix1.cols() + matrix2.cols();

                EIGENTOOLS_ASSERT(number_of_rows == matrix2.rows(), "Inconsistent size of input matrices.");

                result.resize(number_of_rows, number_of_cols);
                result << matrix1, matrix2;
            }
        }
    }



    /**
     * @brief Concatenate matrices horizontally, [A B C ...]
     *
     * @tparam t_DerivedOutput  Eigen parameter
     * @tparam t_DerivedInput1  Eigen parameter
     * @tparam t_DerivedInput2  Eigen parameter
     * @tparam t_DerivedInput3  Eigen parameter
     *
     * @param[out] result    result of concatenation
     * @param[in] matrix1
     * @param[in] matrix2
     * @param[in] matrix3
     */
    template<   class t_DerivedOutput,
                class t_DerivedInput1,
                class t_DerivedInput2,
                class t_DerivedInput3>
        void  EIGENTOOLS_VISIBILITY_ATTRIBUTE
            concatenateMatricesHorizontally(
                Eigen::PlainObjectBase<t_DerivedOutput> &result,
                const Eigen::DenseBase<t_DerivedInput1> &matrix1,
                const Eigen::DenseBase<t_DerivedInput2> &matrix2,
                const Eigen::DenseBase<t_DerivedInput3> &matrix3)
    {
        if (matrix1.rows() == 0)
        {
            concatenateMatricesHorizontally(result, matrix2, matrix3);
        }
        else
        {
            if (matrix2.rows() == 0)
            {
                concatenateMatricesHorizontally(result, matrix1, matrix3);
            }
            else
            {
                if (matrix3.rows() == 0)
                {
                    concatenateMatricesHorizontally(result, matrix1, matrix2);
                }
                else
                {
                    std::ptrdiff_t  number_of_rows = matrix1.rows();
                    std::ptrdiff_t  number_of_cols = matrix1.cols() + matrix2.cols() + matrix3.cols();

                    EIGENTOOLS_ASSERT(  (number_of_rows == matrix2.rows())
                                        && (number_of_rows == matrix3.rows()),
                                        "Inconsistent size of input matrices.");

                    result.resize(number_of_rows, number_of_cols);
                    result << matrix1, matrix2, matrix3;
                }
            }
        }
    }



    /**
     * @brief Remove a row with the specified index.
     *
     * @tparam t_Derived  Eigen parameter
     *
     * @param[in,out] matrix        matrix
     * @param[in] row_to_remove index of a row
     *
     * Based on
     * http://stackoverflow.com/questions/13290395/how-to-remove-a-certain-row-or-column-while-using-eigen-library-c
     */
    template<class t_Derived>
        void  EIGENTOOLS_VISIBILITY_ATTRIBUTE
            removeRow(  Eigen::PlainObjectBase<t_Derived> & matrix,
                        const std::ptrdiff_t row_to_remove)
    {
        EIGENTOOLS_ASSERT(row_to_remove <= matrix.rows(), "The index of a removed row is greater than the size of the matrix.");

        std::ptrdiff_t number_of_rows = matrix.rows()-1;
        std::ptrdiff_t number_of_cols = matrix.cols();

        if( row_to_remove < number_of_rows )
        {
            matrix.block(row_to_remove, 0, number_of_rows - row_to_remove, number_of_cols) =
                matrix.block(row_to_remove + 1, 0 , number_of_rows - row_to_remove, number_of_cols);
        }

        matrix.conservativeResize(number_of_rows, number_of_cols);
    }



    /**
     * @brief Remove a column with the specified index.
     *
     * @tparam t_Derived  Eigen parameter
     *
     * @param[in,out] matrix            matrix
     * @param[in] column_to_remove  index of a column
     *
     * Based on
     * http://stackoverflow.com/questions/13290395/how-to-remove-a-certain-row-or-column-while-using-eigen-library-c
     */
    template<class t_Derived>
        void  EIGENTOOLS_VISIBILITY_ATTRIBUTE
            removeColumn(   Eigen::PlainObjectBase<t_Derived> & matrix,
                            const std::ptrdiff_t column_to_remove)
    {
        EIGENTOOLS_ASSERT(column_to_remove <= matrix.rows(), "The index of a removed column is greater than the size of the matrix.");

        std::ptrdiff_t number_of_rows = matrix.rows();
        std::ptrdiff_t number_of_cols = matrix.cols()-1;

        if( column_to_remove < number_of_cols )
        {
            matrix.block(0, column_to_remove, number_of_rows, number_of_cols - column_to_remove) =
                matrix.block(0, column_to_remove + 1 ,number_of_rows, number_of_cols - column_to_remove);
        }

        matrix.conservativeResize(number_of_rows, number_of_cols);
    }


    /**
     * @brief Custom Kronecker product: blocks of the input matrix are treated as
     * single elements.
     *
     * @tparam t_Derived  Eigen parameter
     *
     * @param[in] input input matrix
     * @param[in] input_block_rows  rows in a block
     * @param[in] input_block_cols  cols in a block
     * @param[in] identity_size     size of the identity matrix used in the product
     *
     * @return result of the product
     */
    template<class t_Derived>
        EIGENTOOLS_DYNAMIC_MATRIX(typename Eigen::DenseBase<t_Derived>::Scalar)
        EIGENTOOLS_VISIBILITY_ATTRIBUTE
            multiplyBlockKroneckerIdentity (const Eigen::DenseBase<t_Derived> & input,
                                            const std::size_t                   input_block_rows,
                                            const std::size_t                   input_block_cols,
                                            const std::size_t                   identity_size)
    {
        EIGENTOOLS_ASSERT(input.rows() % input_block_rows == 0, "Wrong block size.");
        EIGENTOOLS_ASSERT(input.cols() % input_block_cols == 0, "Wrong block size.");

        EIGENTOOLS_DYNAMIC_MATRIX(typename Eigen::DenseBase<t_Derived>::Scalar)  output;

        std::size_t     num_blocks_vert = input.rows() / input_block_rows;
        std::size_t     num_blocks_hor  = input.cols() / input_block_cols;

        std::size_t     output_block_rows = identity_size*input_block_rows;
        std::size_t     output_block_cols = identity_size*input_block_cols;

        output.setZero(identity_size * input.rows(),
                       identity_size * input.cols());

        for (std::size_t i = 0; i < num_blocks_vert; ++i)
        {
            for (std::size_t j = 0; j < num_blocks_hor; ++j)
            {
                for(std::size_t k = 0; k < identity_size; ++k)
                {
                    output.block(   i*output_block_rows + k*input_block_rows,
                                    j*output_block_cols + k*input_block_cols,
                                    input_block_rows,
                                    input_block_cols)
                        = input.block(i*input_block_rows, j*input_block_cols, input_block_rows, input_block_cols);
                }
            }
        }

        return(output);
    }


    // ===========================================================================
    // Selections
    // ===========================================================================


    /**
     * @brief Select rows from a matrix, in Matlab notation the result is
     * M(first:step:end, :).
     *
     * @tparam t_Derived  Eigen parameter
     *
     * @param[in] matrix    input matrix
     * @param[in] row_step  each 'row_step' is selected from the input matrix
     * @param[in] first_row starting from 'first_row'
     *
     * @return Matrix consisting of selected rows.
     */
    template<class t_Derived>
        inline Eigen::Map<  const EIGENTOOLS_DYNAMIC_MATRIX(typename Eigen::PlainObjectBase<t_Derived>::Scalar),
                            Eigen::Unaligned,
                            Eigen::Stride<Eigen::Dynamic, Eigen::Dynamic> >
        EIGENTOOLS_VISIBILITY_ATTRIBUTE
            selectRows( const Eigen::PlainObjectBase<t_Derived> &matrix,
                        const std::size_t row_step,
                        const std::size_t first_row = 0)
    {
        return (EIGENTOOLS_DYNAMIC_MATRIX(typename Eigen::PlainObjectBase<t_Derived>::Scalar)::Map(
                    matrix.data() + first_row,
                    ceil( static_cast<double> (matrix.rows() - first_row)/row_step),
                    matrix.cols(),
                    Eigen::Stride<Eigen::Dynamic, Eigen::Dynamic>(matrix.rows(), row_step)));
    }


    /**
     * @brief Selection matrix
     */
    class EIGENTOOLS_VISIBILITY_ATTRIBUTE SelectionMatrix
    {
        template<class t_Derived>
        friend Eigen::Map<  const EIGENTOOLS_DYNAMIC_MATRIX( typename Eigen::PlainObjectBase<t_Derived>::Scalar ),
                            Eigen::Unaligned,
                            Eigen::Stride<Eigen::Dynamic, Eigen::Dynamic> >
            operator*(   const SelectionMatrix &,
                         const Eigen::PlainObjectBase<t_Derived> &);


        private:
            std::size_t step_size_;
            std::size_t first_index_;

        public:
            /**
             * @brief Constructore
             *
             * @param[in] step_size     step size of selection: 3 -- each third element
             * @param[in] first_index   index of the first element for selection
             */
            SelectionMatrix( const std::size_t step_size,
                             const std::size_t first_index)
            {
                step_size_   = step_size;
                first_index_ = first_index;
            }
    };


    /**
     * @brief Multiply selection matrix by a generic Eigen matrix (select rows)
     *
     * @tparam t_Derived  Eigen parameter
     *
     * @param[in] selector
     * @param[in] matrix
     *
     * @return selected rows
     */
    template<class t_Derived>
        inline Eigen::Map<  const EIGENTOOLS_DYNAMIC_MATRIX( typename Eigen::PlainObjectBase<t_Derived>::Scalar ),
                            Eigen::Unaligned,
                            Eigen::Stride<Eigen::Dynamic, Eigen::Dynamic> >
        EIGENTOOLS_VISIBILITY_ATTRIBUTE
            operator*(  const SelectionMatrix                   & selector,
                        const Eigen::PlainObjectBase<t_Derived> & matrix)
    {
        return(selectRows(matrix, selector.step_size_, selector.first_index_));
    }


    #ifdef EIGENTOOLS_ENABLE_EIGENTYPE_DETECTION
        #include <boost/utility/enable_if.hpp>
        #include <boost/mpl/has_xxx.hpp>
        #include <boost/mpl/and.hpp>

        namespace detail
        {
            BOOST_MPL_HAS_XXX_TRAIT_DEF(Scalar)
            BOOST_MPL_HAS_XXX_TRAIT_DEF(Index)
            BOOST_MPL_HAS_XXX_TRAIT_DEF(StorageKind)
        }


        /**
         * Traits for checking if T is indeed an Eigen Type
         * @tparam T any Type
         *
         * Example Usage:
         * is_eigen_type<int>::value // evaluates to false
         * is_eigen_type<int>::type // evaluates to false_type
         * is_eigen_type<Eigen::Vector2d>::value // evaluates to true
         * is_eigen_type<Eigen::Vector2d>::type // true_type
         *
         * @see https://forum.kde.org/viewtopic.php?f=74&t=121280
         */
        template<typename T>
            struct is_eigen_type : boost::mpl::and_<detail::has_Scalar<T>,
                                                    detail::has_Index<T>,
                                                    detail::has_StorageKind<T> >
        {
        };

        #define EIGENTOOLS_EIGENTYPE_ENABLER_TYPE(DataType)  \
            const typename boost::enable_if_c< etools::is_eigen_type<DataType>::value >::type
        #define EIGENTOOLS_EIGENTYPE_DISABLER_TYPE(DataType) \
            const typename boost::enable_if_c< !(etools::is_eigen_type<DataType>::value) >::type
    #endif
} // etools
