/**
    @file
    @author  Alexander Sherikov
    @copyright 2014-2017 INRIA. Licensed under the Apache License, Version 2.0.
    (see @ref LICENSE or http://www.apache.org/licenses/LICENSE-2.0)

    @brief
*/

#pragma once

namespace etools
{
    /**
     * @brief Type modifier (drop reference &)
     *
     * @tparam T input type
     */
    template<class T>   struct TypeWithoutReference
    {
        typedef T Type;
    };

    /**
     * @brief Type modifier (drop reference &)
     *
     * @tparam T input type
     */
    template<class T>   struct TypeWithoutReference<T&>
    {
        typedef T Type;
    };


    /**
     * @brief Type modifier (drop reference 'const')
     *
     * @tparam T input type
     */
    template<class T>   struct TypeWithoutConst
    {
        typedef T Type;
    };

    /**
     * @brief Type modifier (drop 'const')
     *
     * @tparam T input type
     */
    template<class T>   struct TypeWithoutConst<const T>
    {
        typedef T Type;
    };


    /**
     * @brief Type modifier (drop reference and 'const')
     *
     * @tparam T input type
     */
    template<class T>   struct TypeDecayed
    {
        typedef typename TypeWithoutConst<typename TypeWithoutReference<T>::Type>::Type Type;
    };



    /**
     * @brief Matrix block size type
     */
    class MatrixBlockSizeType
    {
        public:
            enum Type
            {
                UNDEFINED = 0,
                DYNAMIC = -1
            };
    };

#define EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM   ((t_block_rows_num > 0) ? t_block_rows_num : block_rows_num_)
#define EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM   ((t_block_cols_num > 0) ? t_block_cols_num : block_cols_num_)

#define     EIGENTOOLS_DEFINE_BLOCK_MATRIX_CONSTRUCTORS(class_name) \
            class_name (const std::ptrdiff_t  block_rows_num = MatrixBlockSizeType::UNDEFINED,\
                        const std::ptrdiff_t  block_cols_num = MatrixBlockSizeType::UNDEFINED)\
                : EIGENTOOLS_PARENT_CLASS_SHORTHAND(block_rows_num, block_cols_num) {};\
            class_name (const DecayedRawMatrix & matrix,\
                        const std::ptrdiff_t  block_rows_num = MatrixBlockSizeType::UNDEFINED,\
                        const std::ptrdiff_t  block_cols_num = MatrixBlockSizeType::UNDEFINED)\
                : EIGENTOOLS_PARENT_CLASS_SHORTHAND(matrix, block_rows_num, block_cols_num) {};\
            class_name (DecayedRawMatrix & matrix,\
                        const std::ptrdiff_t  block_rows_num = MatrixBlockSizeType::UNDEFINED,\
                        const std::ptrdiff_t  block_cols_num = MatrixBlockSizeType::UNDEFINED)\
                : EIGENTOOLS_PARENT_CLASS_SHORTHAND(matrix, block_rows_num, block_cols_num) {};

    /**
     * @brief Block matrix basic functions
     *
     * @tparam t_MatrixType     type of raw matrix
     * @tparam t_block_rows_num number of rows in one block
     * @tparam t_block_cols_num number of columns in one block
     */
    template<   typename t_MatrixType,
                int t_block_rows_num,
                int t_block_cols_num>
    class EIGENTOOLS_VISIBILITY_ATTRIBUTE BlockMatrixAccessBase
    {
        public:
            /// Raw matrix without 'const' and '&'
            typedef typename TypeDecayed<t_MatrixType>::Type     DecayedRawMatrix;

            /// Scalar type of raw matrix (from Eigen)
            typedef typename DecayedRawMatrix::Scalar           Scalar;

            /// Shorthand for Eigen block
            typedef Eigen::Block< EIGENTOOLS_DYNAMIC_MATRIX( Scalar ) > DynamicMatrixBlock;

            /// Shorthand for Eigen block
            typedef const Eigen::Block< const EIGENTOOLS_DYNAMIC_MATRIX( Scalar ) > ConstDynamicMatrixBlock;


        public:
            /**
             * @brief Get number of blocks (horizontal/vertical)
             *
             * @return number of blocks
             */
            std::ptrdiff_t getNumberOfBlocksVertical() const
            {
                return(num_blocks_vert_);
            }


            /// @copydoc getNumberOfBlocksVertical()
            std::ptrdiff_t getNumberOfBlocksHorizontal() const
            {
                return(num_blocks_hor_);
            }



            /**
             * @brief Get total number of rows / columns
             *
             * @return number of rows / columns
             */
            std::ptrdiff_t getNumberOfRows() const
            {
                return(EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM * num_blocks_vert_);
            }


            /// @copydoc getNumberOfRows()
            std::ptrdiff_t getNumberOfColumns() const
            {
                return(EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM * num_blocks_hor_);
            }


            /**
             * @brief Set raw matrix
             *
             * @tparam t_Derived    Eigen parameter
             *
             * @param[in] matrix
             */
            template <class t_Derived>
                void set (const Eigen::DenseBase<t_Derived> &matrix)
            {
                matrix_ = matrix;
                finalize();
            }


            /**
             * @brief Resize matrix
             *
             * @param[in] num_blocks_vert
             * @param[in] num_blocks_hor
             */
            void resize(    const std::ptrdiff_t   num_blocks_vert,
                            const std::ptrdiff_t   num_blocks_hor)
            {
                matrix_.resize( num_blocks_vert*EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM,
                                num_blocks_hor*EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM);
                finalize();
            }


            /**
             * @brief Resize matrix and initialize it with zeros
             *
             * @param[in] num_blocks_vert
             * @param[in] num_blocks_hor
             */
            void setZero(   const std::ptrdiff_t   num_blocks_vert,
                            const std::ptrdiff_t   num_blocks_hor)
            {
                matrix_.setZero(num_blocks_vert*EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM,
                                num_blocks_hor*EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM);
                finalize();
            }


            /**
             * @brief Resize square matrix
             *
             * @param[in] num_blocks number of diagonal blocks
             */
            void resize(const std::ptrdiff_t   num_blocks)
            {
                resize(num_blocks, num_blocks);
            }


            /**
             * @brief Resize square matrix and set it to zero
             *
             * @param[in] num_blocks number of diagonal blocks
             */
            void setZero(const std::ptrdiff_t   num_blocks)
            {
                setZero(num_blocks, num_blocks);
            }



            /**
             * @brief Access column of a matrix
             *
             * @param[in] index_col         index of the column
             * @param[in] index_row_first   segment of the column starts at this row
             * @param[in] index_num_rows    number of blocks
             *
             * @return column or a part of it
             */
            DynamicMatrixBlock column(  const std::ptrdiff_t index_col,
                                        const std::ptrdiff_t index_row_first,
                                        const std::ptrdiff_t index_num_rows)
            {
                return (DynamicMatrixBlock (matrix_.derived(),
                                            index_row_first * EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM,
                                            index_col       * EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM,
                                            index_num_rows  * EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM,
                                            EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM));
            }


            /**
             * @brief Access column of a matrix
             *
             * @param[in] index_col         index of the column
             * @param[in] index_row_first   segment of the column starts at this row
             *                              if #index_row_first is not specified =>
             *                              return the whole column
             *
             * @return column or a part of it
             */
            DynamicMatrixBlock column(  const std::ptrdiff_t index_col,
                                        const std::ptrdiff_t index_row_first = 0)
            {
                return (column(index_col, index_row_first, num_blocks_vert_ - index_row_first));
            }


            /// @copydoc column(const std::ptrdiff_t, const std::ptrdiff_t, const std::ptrdiff_t)
            ConstDynamicMatrixBlock column( const std::ptrdiff_t index_col,
                                            const std::ptrdiff_t index_row_first,
                                            const std::ptrdiff_t index_num_rows) const
            {
                return (ConstDynamicMatrixBlock (   matrix_.derived(),
                                                    index_row_first * EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM,
                                                    index_col       * EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM,
                                                    index_num_rows  * EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM,
                                                    EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM));
            }


            /// @copydoc column(const std::ptrdiff_t, const std::ptrdiff_t)
            ConstDynamicMatrixBlock column( const std::ptrdiff_t index_col,
                                            const std::ptrdiff_t index_row_first = 0) const
            {
                return (column(index_col, index_row_first, num_blocks_vert_ - index_row_first));
            }


            /**
             * @brief Access row of a matrix
             *
             * @param[in] index_row         index of the row
             * @param[in] index_col_first   segment of the row starts at this column
             * @param[in] index_num_cols    number of blocks
             *
             * @return row or a part of it
             */
            DynamicMatrixBlock row( const std::ptrdiff_t index_row,
                                    const std::ptrdiff_t index_col_first,
                                    const std::ptrdiff_t index_num_cols)
            {
                return (DynamicMatrixBlock( matrix_.derived(),
                                            index_row       * EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM,
                                            index_col_first * EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM,
                                            EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM,
                                            index_num_cols  * EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM));
            }


            /**
             * @brief Access row of a matrix
             *
             * @param[in] index_row         index of the row
             * @param[in] index_col_first   segment of the row starts at this column
             *                              if #index_col_first is not specified =>
             *                              return the whole row
             *
             * @return row or a part of it
             */
            DynamicMatrixBlock row( const std::ptrdiff_t index_row,
                                    const std::ptrdiff_t index_col_first = 0)
            {
                return (row(index_row, index_col_first, num_blocks_hor_ - index_col_first));
            }


            /// @copydoc row(const std::ptrdiff_t, const std::ptrdiff_t, const std::ptrdiff_t)
            ConstDynamicMatrixBlock row(const std::ptrdiff_t index_row,
                                        const std::ptrdiff_t index_col_first,
                                        const std::ptrdiff_t index_num_cols) const
            {
                return (ConstDynamicMatrixBlock(matrix_.derived(),
                                                index_row       * EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM,
                                                index_col_first * EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM,
                                                EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM,
                                                index_num_cols  * EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM));
            }


            /// @copydoc row(const std::ptrdiff_t, const std::ptrdiff_t)
            ConstDynamicMatrixBlock row(const std::ptrdiff_t index_row,
                                        const std::ptrdiff_t index_col_first = 0) const
            {
                return (row(index_row, index_col_first, num_blocks_hor_ - index_col_first));
            }



            /**
             * @brief Get raw matrix
             *
             * @return matrix
             */
            const DecayedRawMatrix & getRaw() const
            {
                return (matrix_);
            }


            /**
             * @brief Selects rows from the matrix
             *
             * @param[in] row_in_a_block row number in a block
             *
             * @return selected rows
             */
            DecayedRawMatrix selectRowInBlocksAsMatrix(const std::ptrdiff_t row_in_a_block)
            {
                return(selectRows(matrix_, EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM, row_in_a_block));
            }


            /**
             * @brief Returns dimension of the matrix block.
             *
             * @return number of rows / columns
             */
            std::ptrdiff_t  getBlockRowsNum() const
            {
                return(EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM);
            }


            /// @copydoc getBlockRowsNum
            std::ptrdiff_t  getBlockColsNum() const
            {
                return(EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM);
            }


            /**
             * @brief Returns dimension of the matrix block.
             *
             * @return number of rows / columns
             */
            void setBlockSize(  const std::ptrdiff_t  block_rows_num,
                                const std::ptrdiff_t  block_cols_num)
            {
                EIGENTOOLS_ASSERT(  (MatrixBlockSizeType::DYNAMIC != t_block_rows_num) || (block_rows_num > 0),
                                    "Block dimension must be strictly positive.");
                EIGENTOOLS_ASSERT(  (MatrixBlockSizeType::DYNAMIC != t_block_cols_num) || (block_cols_num > 0),
                                    "Block dimension must be strictly positive.");
                initializeBlockSize(block_rows_num, block_cols_num);
                finalize();
            }


        protected:
            t_MatrixType    matrix_;

            std::ptrdiff_t  num_blocks_vert_;
            std::ptrdiff_t  num_blocks_hor_;

            std::ptrdiff_t  block_rows_num_;
            std::ptrdiff_t  block_cols_num_;


        protected:
            /**
             * @brief Protected destructor: prevent destruction of the child
             * classes through a base pointer.
             */
            ~BlockMatrixAccessBase() {}


            /**
             * @brief Default constructor
             */
            BlockMatrixAccessBase(  const std::ptrdiff_t  block_rows_num = MatrixBlockSizeType::UNDEFINED,
                                    const std::ptrdiff_t  block_cols_num = MatrixBlockSizeType::UNDEFINED)
            {
                initializeBlockSize(block_rows_num, block_cols_num);
                finalize();
            }


            /**
             * @brief Constructor with matrix initialization.
             *
             * @param[in] matrix            matrix
             * @param[in] block_rows_num    number of rows in a block if    t_block_rows_num = MatrixBlockSizeType::DYNAMIC
             * @param[in] block_cols_num    number of cols in a block if    t_block_cols_num = MatrixBlockSizeType::DYNAMIC
             */
            BlockMatrixAccessBase(  const DecayedRawMatrix & matrix,
                                    const std::ptrdiff_t  block_rows_num = MatrixBlockSizeType::UNDEFINED,
                                    const std::ptrdiff_t  block_cols_num = MatrixBlockSizeType::UNDEFINED)
                : matrix_(matrix)
            {
                initializeBlockSize(block_rows_num, block_cols_num);
                finalize();
            }


            /**
             * @brief Constructor with matrix initialization.
             *
             * @param[in] matrix            matrix
             * @param[in] block_rows_num    number of rows in a block if    t_block_rows_num = MatrixBlockSizeType::DYNAMIC
             * @param[in] block_cols_num    number of cols in a block if    t_block_cols_num = MatrixBlockSizeType::DYNAMIC
             */
            BlockMatrixAccessBase(  DecayedRawMatrix & matrix,
                                    const std::ptrdiff_t  block_rows_num = MatrixBlockSizeType::UNDEFINED,
                                    const std::ptrdiff_t  block_cols_num = MatrixBlockSizeType::UNDEFINED)
                : matrix_(matrix)
            {
                initializeBlockSize(block_rows_num, block_cols_num);
                finalize();
            }



            /**
             * @brief Compute humber of blocks in matrix and check size consistency.
             */
            void initializeBlockSize(   const std::ptrdiff_t  block_rows_num,
                                        const std::ptrdiff_t  block_cols_num)
            {
                if (MatrixBlockSizeType::DYNAMIC == t_block_rows_num)
                {
                    block_rows_num_ = block_rows_num;
                }
                else
                {
                    EIGENTOOLS_ASSERT(  (MatrixBlockSizeType::UNDEFINED == block_rows_num),
                                        "Matrix block is static, but nonzero dimension size is passed to the constructor.");
                    block_rows_num_ = t_block_rows_num;
                }

                if (MatrixBlockSizeType::DYNAMIC == t_block_cols_num)
                {
                    block_cols_num_ = block_cols_num;
                }
                else
                {
                    EIGENTOOLS_ASSERT(  (MatrixBlockSizeType::UNDEFINED == block_cols_num),
                                        "Matrix block is static, but nonzero dimension size is passed to the constructor.");
                    block_cols_num_ = t_block_cols_num;
                }
            }


            /**
             * @brief Compute humber of blocks in matrix and check size consistency.
             */
            void finalize()
            {
                if (    (t_block_rows_num != MatrixBlockSizeType::DYNAMIC)
                        || (block_rows_num_ != MatrixBlockSizeType::UNDEFINED))
                {
                    EIGENTOOLS_ASSERT(  (EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM > 0),
                                        "Wrong number of rows.");

                    EIGENTOOLS_ASSERT(  (matrix_.rows() % EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM == 0),
                                        "Vertical dimension of the matrix is not a multiple of the corresponding block dimension.");

                    num_blocks_vert_ = matrix_.rows() / EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM;
                }

                if (    (t_block_cols_num != MatrixBlockSizeType::DYNAMIC)
                        || (block_cols_num_ != MatrixBlockSizeType::UNDEFINED))
                {
                    EIGENTOOLS_ASSERT(  (EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM > 0),
                                        "Wrong number of cols.");

                    EIGENTOOLS_ASSERT(  (matrix_.cols() % EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM == 0),
                                        "Horizontal dimension of the matrix is not a multiple of the corresponding block dimension.");

                    num_blocks_hor_ = matrix_.cols() / EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM;
                }
            }
    };


#define EIGENTOOLS_PARENT_CLASS_SHORTHAND   BlockMatrixAccessBase<t_MatrixType, t_block_rows_num, t_block_cols_num>
    /**
     * @brief Extra layer for handling of specific sizes of blocks using partial
     * template specialization
     *
     * @tparam t_MatrixType     type of raw matrix
     * @tparam t_block_rows_num number of rows in one block
     * @tparam t_block_cols_num number of columns in one block
     */
    template<   typename t_MatrixType,
                int t_block_rows_num,
                int t_block_cols_num>
    class EIGENTOOLS_VISIBILITY_ATTRIBUTE BlockMatrixSizeSpecificBase : public EIGENTOOLS_PARENT_CLASS_SHORTHAND
    {
        public:
            typedef typename EIGENTOOLS_PARENT_CLASS_SHORTHAND::DecayedRawMatrix DecayedRawMatrix;
            typedef typename EIGENTOOLS_PARENT_CLASS_SHORTHAND::Scalar           Scalar;


            /// Shorthand for Eigen block
            typedef Eigen::Block<   EIGENTOOLS_DYNAMIC_MATRIX( Scalar ),
                                    t_block_rows_num,
                                    t_block_cols_num> StaticMatrixBlock;

            /// Shorthand for Eigen block
            typedef const Eigen::Block< const EIGENTOOLS_DYNAMIC_MATRIX( Scalar ),
                                        t_block_rows_num,
                                        t_block_cols_num> ConstStaticMatrixBlock;

            /**
             * @brief Block access operator
             *
             * @param[in] index_row
             * @param[in] index_col
             *
             * @return matrix block
             */
            StaticMatrixBlock operator()(   const std::ptrdiff_t index_row,
                                            const std::ptrdiff_t index_col)
            {
                return (StaticMatrixBlock(  matrix_.derived(),
                                            index_row * t_block_rows_num,
                                            index_col * t_block_cols_num));
            }


            /// @copydoc operator()(const std::ptrdiff_t, const std::ptrdiff_t)
            ConstStaticMatrixBlock  operator()( const std::ptrdiff_t index_row,
                                                const std::ptrdiff_t index_col) const
            {
                return (ConstStaticMatrixBlock( matrix_.derived(),
                                                index_row * t_block_rows_num,
                                                index_col * t_block_cols_num));
            }


            /**
             * @brief Block access operator for diagonal blocks
             *
             * @param[in] index row and column index
             *
             * @return matrix block
             */
            StaticMatrixBlock operator()(const std::ptrdiff_t index)
            {
                return ((*this)(index,index));
            }


            /// @copydoc operator()(const std::ptrdiff_t)
            ConstStaticMatrixBlock    operator()(const std::ptrdiff_t index) const
            {
                return ((*this)(index,index));
            }


        protected:
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::matrix_;


            /**
             * @brief Protected destructor: prevent destruction of the child
             * classes through a base pointer.
             */
            ~BlockMatrixSizeSpecificBase() {}

            EIGENTOOLS_DEFINE_BLOCK_MATRIX_CONSTRUCTORS(BlockMatrixSizeSpecificBase)
    };
#undef EIGENTOOLS_PARENT_CLASS_SHORTHAND


#define EIGENTOOLS_CODE_GENERATOR\
        public:\
            typedef typename EIGENTOOLS_PARENT_CLASS_SHORTHAND::DecayedRawMatrix        DecayedRawMatrix;\
            typedef typename EIGENTOOLS_PARENT_CLASS_SHORTHAND::DynamicMatrixBlock      DynamicMatrixBlock;\
            typedef typename EIGENTOOLS_PARENT_CLASS_SHORTHAND::ConstDynamicMatrixBlock ConstDynamicMatrixBlock;\
            DynamicMatrixBlock  operator()( const std::ptrdiff_t index_row,\
                                            const std::ptrdiff_t index_col)\
            {\
                return (DynamicMatrixBlock( matrix_.derived(),\
                                            index_row * block_rows_num_,\
                                            index_col * block_cols_num_,\
                                            block_rows_num_,\
                                            block_cols_num_));\
            }\
            ConstDynamicMatrixBlock operator()( const std::ptrdiff_t index_row,\
                                                const std::ptrdiff_t index_col) const\
            {\
                return (ConstDynamicMatrixBlock(matrix_.derived(),\
                                                index_row * block_rows_num_,\
                                                index_col * block_cols_num_,\
                                                block_rows_num_,\
                                                block_cols_num_));\
            }\
            DynamicMatrixBlock\
                operator()(const std::ptrdiff_t index)\
            {\
                return ((*this)(index,index));\
            }\
            ConstDynamicMatrixBlock\
                operator()(const std::ptrdiff_t index) const\
            {\
                return ((*this)(index,index));\
            }\
        protected:\
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::matrix_;\
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::block_rows_num_;\
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::block_cols_num_;\
            ~BlockMatrixSizeSpecificBase() {}\
            EIGENTOOLS_DEFINE_BLOCK_MATRIX_CONSTRUCTORS(BlockMatrixSizeSpecificBase)\

#define EIGENTOOLS_PARENT_CLASS_SHORTHAND   BlockMatrixAccessBase<  t_MatrixType, \
                                                                    MatrixBlockSizeType::DYNAMIC, \
                                                                    MatrixBlockSizeType::DYNAMIC>

    /**
     * @brief Extra layer for handling of specific sizes of blocks using partial
     * template specialization
     *
     * @tparam t_MatrixType     type of raw matrix
     * @tparam t_block_rows_num number of rows in one block
     * @tparam t_block_cols_num number of columns in one block
     */
    template<typename t_MatrixType>
        class EIGENTOOLS_VISIBILITY_ATTRIBUTE BlockMatrixSizeSpecificBase<  t_MatrixType,
                                                                            MatrixBlockSizeType::DYNAMIC,
                                                                            MatrixBlockSizeType::DYNAMIC>
            : public EIGENTOOLS_PARENT_CLASS_SHORTHAND
    {
        EIGENTOOLS_CODE_GENERATOR
    };
#undef EIGENTOOLS_PARENT_CLASS_SHORTHAND


#define EIGENTOOLS_PARENT_CLASS_SHORTHAND   BlockMatrixAccessBase<  t_MatrixType, \
                                                                    t_block_rows_num, \
                                                                    MatrixBlockSizeType::DYNAMIC>
    /**
     * @brief Extra layer for handling of specific sizes of blocks using partial
     * template specialization
     *
     * @tparam t_MatrixType     type of raw matrix
     * @tparam t_block_rows_num number of rows in one block
     * @tparam t_block_cols_num number of columns in one block
     */
    template<typename t_MatrixType, int t_block_rows_num>
        class EIGENTOOLS_VISIBILITY_ATTRIBUTE BlockMatrixSizeSpecificBase<  t_MatrixType,
                                                                            t_block_rows_num,
                                                                            MatrixBlockSizeType::DYNAMIC>
            : public EIGENTOOLS_PARENT_CLASS_SHORTHAND
    {
        EIGENTOOLS_CODE_GENERATOR
    };
#undef EIGENTOOLS_PARENT_CLASS_SHORTHAND


#define EIGENTOOLS_PARENT_CLASS_SHORTHAND   BlockMatrixAccessBase<  t_MatrixType, \
                                                                    MatrixBlockSizeType::DYNAMIC, \
                                                                    t_block_cols_num>
    /**
     * @brief Extra layer for handling of specific sizes of blocks using partial
     * template specialization
     *
     * @tparam t_MatrixType     type of raw matrix
     * @tparam t_block_rows_num number of rows in one block
     * @tparam t_block_cols_num number of columns in one block
     */
    template<typename t_MatrixType, int t_block_cols_num>
        class EIGENTOOLS_VISIBILITY_ATTRIBUTE BlockMatrixSizeSpecificBase<  t_MatrixType,
                                                                            MatrixBlockSizeType::DYNAMIC,
                                                                            t_block_cols_num>
            : public EIGENTOOLS_PARENT_CLASS_SHORTHAND
    {
        EIGENTOOLS_CODE_GENERATOR
    };
#undef EIGENTOOLS_PARENT_CLASS_SHORTHAND
#undef EIGENTOOLS_CODE_GENERATOR


#define EIGENTOOLS_PARENT_CLASS_SHORTHAND     BlockMatrixAccessBase<t_MatrixType, 1, 1>
    /**
     * @brief Extra layer for handling of specific sizes of blocks using partial
     * template specialization
     *
     * Access matrix using 1x1 blocks. The reason for this is that Eigen does not
     * support Block's of such size.
     *
     * @tparam t_MatrixType     type of raw matrix, e.g. Eigen::MatrixXd or a reference to it
     */
    template<typename t_MatrixType>
    class EIGENTOOLS_VISIBILITY_ATTRIBUTE BlockMatrixSizeSpecificBase<t_MatrixType, 1, 1> : public EIGENTOOLS_PARENT_CLASS_SHORTHAND
    {
        public:
            typedef typename EIGENTOOLS_PARENT_CLASS_SHORTHAND::DecayedRawMatrix    DecayedRawMatrix;
            typedef typename EIGENTOOLS_PARENT_CLASS_SHORTHAND::Scalar              Scalar;


            /**
             * @brief Block access operator
             *
             * @param[in] index_row
             * @param[in] index_col
             *
             * @return matrix block
             */
            Scalar & operator()(const std::ptrdiff_t index_row,
                                const std::ptrdiff_t index_col)
            {
                return (matrix_(index_row, index_col));
            }


            /// @copydoc operator()(const std::ptrdiff_t, const std::ptrdiff_t)
            Scalar operator()(  const std::ptrdiff_t index_row,
                                const std::ptrdiff_t index_col) const
            {
                return (matrix_(index_row, index_col));
            }


            /**
             * @brief Block access operator for diagonal blocks
             *
             * @param[in] index row and column index
             *
             * @return matrix block
             */
            Scalar & operator()(const std::ptrdiff_t index)
            {
                return (matrix_(index, index));
            }


            /// @copydoc operator()(const std::ptrdiff_t)
            Scalar operator()(  const std::ptrdiff_t index) const
            {
                return (matrix_(index, index));
            }


        protected:
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::matrix_;


            /**
             * @brief Protected destructor: prevent destruction of the child
             * classes through a base pointer.
             */
            ~BlockMatrixSizeSpecificBase() {}


            EIGENTOOLS_DEFINE_BLOCK_MATRIX_CONSTRUCTORS(BlockMatrixSizeSpecificBase)
    };
#undef EIGENTOOLS_PARENT_CLASS_SHORTHAND


    // ===========================================================================
    // ===========================================================================
    // ===========================================================================


    /**
     * @brief Sparsity type of a matrix
     */
    class MatrixSparsityType
    {
        public:
            enum Type
            {
                UNDEFINED = 0,
                NONE = 1,
                DIAGONAL = 2,
                LEFT_LOWER_TRIANGULAR = 3
            };
    };


    // ===========================================================================
    // ===========================================================================
    // ===========================================================================


#define EIGENTOOLS_PARENT_CLASS_SHORTHAND     BlockMatrixSizeSpecificBase<t_MatrixType, t_block_rows_num, t_block_cols_num>
    /**
     * @brief Base class of a block matrix
     *
     * @tparam t_MatrixType     type of raw matrix
     * @tparam t_block_rows_num number of rows in one block
     * @tparam t_block_cols_num number of columns in one block
     * @tparam t_sparsity_type  sparsity type
     */
    template<   typename t_MatrixType,
                int t_block_rows_num,
                int t_block_cols_num,
                MatrixSparsityType::Type t_sparsity_type>
    class EIGENTOOLS_VISIBILITY_ATTRIBUTE BlockMatrixBase : public EIGENTOOLS_PARENT_CLASS_SHORTHAND
    {
        protected:
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::matrix_;


        public:
            typedef typename EIGENTOOLS_PARENT_CLASS_SHORTHAND::DecayedRawMatrix    DecayedRawMatrix;

            EIGENTOOLS_DEFINE_BLOCK_MATRIX_CONSTRUCTORS(BlockMatrixBase)


            /**
             * @brief this * Matrix
             *
             * @tparam t_DerivedOutput  Eigen parameter
             * @tparam t_DerivedInput   Eigen parameter
             *
             * @param[out] result result of multiplication
             * @param[in] matrix
             */
            template<class t_DerivedOutput, class t_DerivedInput>
                void multiplyRight (Eigen::PlainObjectBase<t_DerivedOutput> & result,
                                    const Eigen::MatrixBase<t_DerivedInput> & matrix) const
            {
                result.noalias() = matrix_*matrix;
            }
    };
#undef EIGENTOOLS_PARENT_CLASS_SHORTHAND


#define EIGENTOOLS_PARENT_CLASS_SHORTHAND     BlockMatrixSizeSpecificBase<t_MatrixType, t_block_rows_num, t_block_cols_num>
    /**
     * @brief Base class of a block matrix
     *
     * @tparam t_MatrixType     type of raw matrix
     * @tparam t_block_rows_num number of rows in one block
     * @tparam t_block_cols_num number of columns in one block
     * @tparam t_sparsity_type  sparsity type
     */
    template<   typename t_MatrixType,
                int t_block_rows_num,
                int t_block_cols_num >
    class EIGENTOOLS_VISIBILITY_ATTRIBUTE BlockMatrixBase<  t_MatrixType,
                                                            t_block_rows_num,
                                                            t_block_cols_num,
                                                            MatrixSparsityType::DIAGONAL > : public EIGENTOOLS_PARENT_CLASS_SHORTHAND
    {
        public:
            typedef typename EIGENTOOLS_PARENT_CLASS_SHORTHAND::DecayedRawMatrix    DecayedRawMatrix;

            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::operator();
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::getNumberOfRows;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::getNumberOfColumns;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::column;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::row;



            /**
             * @brief this * Matrix
             *
             * @tparam t_DerivedOutput  Eigen parameter
             * @tparam t_DerivedInput   Eigen parameter
             *
             * @param[out] result result of multiplication
             * @param[in] matrix
             */
            template<class t_DerivedOutput, class t_DerivedInput>
                void multiplyRight (Eigen::PlainObjectBase<t_DerivedOutput>     & result,
                                    const Eigen::MatrixBase<t_DerivedInput>     & matrix) const
            {
                EIGENTOOLS_ASSERT(matrix.rows() == getNumberOfColumns(), "Size mismatch.");

                std::ptrdiff_t num_cols = matrix.cols();

                result.resize(getNumberOfRows(), num_cols);

                for(std::ptrdiff_t i = 0; i < num_blocks_hor_; ++i)
                {
                    result.block(i*EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM, 0, EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM, num_cols).noalias() =
                        (*this)(i)
                        *
                        matrix.block(i*EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM, 0, EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM, num_cols);
                }
            }


            /**
             * @brief Matrix * BlockMatrix<DIAGONAL>
             *
             * @tparam t_DerivedInput  Eigen parameter
             * @tparam t_DerivedOutput Eigen parameter
             *
             * @param[out] result result of multiplication
             * @param[in] matrix
             */
            template<   class t_DerivedInput,
                        class t_DerivedOutput>
                void multiplyLeft(  Eigen::PlainObjectBase<t_DerivedOutput> &result,
                                    const Eigen::DenseBase<t_DerivedInput>       &matrix) const
            {
                EIGENTOOLS_ASSERT(matrix.cols() == getNumberOfRows(), "Size mismatch.");

                std::ptrdiff_t num_rows = matrix.rows();

                result.resize(num_rows, getNumberOfColumns());

                for(std::ptrdiff_t i = 0; i < num_blocks_vert_; ++i)
                {
                    result.block(0, i*EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM, num_rows, EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM).noalias() =
                        matrix.block(0, i*EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM, num_rows, EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM)
                        *
                        (*this)(i);
                }
            }


            /**
             * @brief this * BlockMatrix
             *
             * @tparam t_DerivedOutput  Eigen parameter
             * @tparam t_DerivedInput   Eigen parameter
             *
             * @param[out] result   result of multiplication
             * @param[in] rhs       right hand side
             */
            template<   typename t_MatrixType_in,
                        int t_block_rows_num_in,
                        int t_block_cols_num_in,
                        MatrixSparsityType::Type t_sparsity_type_in,
                        class t_DerivedOutput>
                void multiplyRight (Eigen::PlainObjectBase<t_DerivedOutput>         & result,
                                    const BlockMatrixBase<  t_MatrixType_in,
                                                            t_block_rows_num_in,
                                                            t_block_cols_num_in,
                                                            t_sparsity_type_in>     & rhs) const
            {
                multiplyRight(result, rhs.getRaw());
            }


        protected:
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::matrix_;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::num_blocks_vert_;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::num_blocks_hor_;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::block_rows_num_;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::block_cols_num_;


            /**
             * @brief Protected destructor: prevent destruction of the child
             * classes through a base pointer.
             */
            ~BlockMatrixBase() {}


            EIGENTOOLS_DEFINE_BLOCK_MATRIX_CONSTRUCTORS(BlockMatrixBase)
    };
#undef EIGENTOOLS_PARENT_CLASS_SHORTHAND



#define EIGENTOOLS_PARENT_CLASS_SHORTHAND     BlockMatrixSizeSpecificBase<t_MatrixType, t_block_rows_num, t_block_cols_num>
    /**
     * @brief Base class of a block matrix
     *
     * @tparam t_MatrixType     type of raw matrix
     * @tparam t_block_rows_num number of rows in one block
     * @tparam t_block_cols_num number of columns in one block
     * @tparam t_sparsity_type  sparsity type
     */
    template<   typename t_MatrixType,
                int t_block_rows_num,
                int t_block_cols_num >
    class EIGENTOOLS_VISIBILITY_ATTRIBUTE BlockMatrixBase<  t_MatrixType,
                                                            t_block_rows_num,
                                                            t_block_cols_num,
                                                            MatrixSparsityType::LEFT_LOWER_TRIANGULAR> : public EIGENTOOLS_PARENT_CLASS_SHORTHAND
    {
        public:
            typedef typename EIGENTOOLS_PARENT_CLASS_SHORTHAND::DecayedRawMatrix    DecayedRawMatrix;

            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::operator();
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::getNumberOfRows;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::getNumberOfColumns;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::column;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::row;



            /**
             * @brief this * Matrix
             *
             * @tparam t_DerivedOutput  Eigen parameter
             * @tparam t_DerivedInput   Eigen parameter
             *
             * @param[out] result result of multiplication
             * @param[in] matrix
             */
            template<class t_DerivedOutput, class t_DerivedInput>
                void multiplyRight (Eigen::PlainObjectBase<t_DerivedOutput> & result,
                                    const Eigen::MatrixBase<t_DerivedInput> & matrix) const
            {
                // slower
                /*
                EIGENTOOLS_ASSERT(static_cast<std::ptrdiff_t>(eigen_matrix.rows()) == getNumberOfColumns(), "Size mismatch.");

                std::ptrdiff_t num_cols = eigen_matrix.cols();

                result.resize(getNumberOfRows(), num_cols);


                for(std::ptrdiff_t i = 0; i < num_blocks_hor_; ++i)
                {
                    result.block(i*EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM, 0, EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM, num_cols).noalias() =
                        row(i, 0, i+1)
                        *
                        eigen_matrix.block(0, 0, EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM*(i+1), num_cols);
                }
                */
                result.noalias() = matrix_*matrix;
            }


        protected:
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::matrix_;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::num_blocks_vert_;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::num_blocks_hor_;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::block_rows_num_;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::block_cols_num_;


            /**
             * @brief Protected destructor: prevent destruction of the child
             * classes through a base pointer.
             */
            ~BlockMatrixBase() {}


            EIGENTOOLS_DEFINE_BLOCK_MATRIX_CONSTRUCTORS(BlockMatrixBase)
    };
#undef EIGENTOOLS_PARENT_CLASS_SHORTHAND



#define EIGENTOOLS_PARENT_CLASS_SHORTHAND     BlockMatrixSizeSpecificBase<t_MatrixType, 1, 1>
    /**
     * @brief Base class of a block matrix
     *
     * @tparam t_MatrixType     type of raw matrix
     * @tparam t_block_rows_num number of rows in one block
     * @tparam t_block_cols_num number of columns in one block
     * @tparam t_sparsity_type  sparsity type
     */
    template<   typename t_MatrixType >
    class EIGENTOOLS_VISIBILITY_ATTRIBUTE BlockMatrixBase<  t_MatrixType,
                                                            1,
                                                            1,
                                                            MatrixSparsityType::LEFT_LOWER_TRIANGULAR> : public EIGENTOOLS_PARENT_CLASS_SHORTHAND
    {
        public:
            typedef typename EIGENTOOLS_PARENT_CLASS_SHORTHAND::DecayedRawMatrix    DecayedRawMatrix;

            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::operator();
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::getNumberOfRows;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::getNumberOfColumns;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::column;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::row;



            /**
             * @brief this * Matrix
             *
             * @tparam t_DerivedOutput  Eigen parameter
             * @tparam t_DerivedInput   Eigen parameter
             *
             * @param[out] result result of multiplication
             * @param[in] matrix
             */
            template<class t_DerivedOutput, class t_DerivedInput>
                void multiplyRight (Eigen::PlainObjectBase<t_DerivedOutput> & result,
                                    const Eigen::MatrixBase<t_DerivedInput> & matrix) const
            {
                EIGENTOOLS_ASSERT(matrix.rows() == getNumberOfColumns(), "Size mismatch.");

                // https://eigen.tuxfamily.org/dox-devel/TopicTemplateKeyword.html
                result.noalias() = matrix_.template triangularView<Eigen::Lower>()  * matrix;
            }


        protected:
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::matrix_;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::num_blocks_vert_;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::num_blocks_hor_;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::block_rows_num_;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::block_cols_num_;


            /**
             * @brief Protected destructor: prevent destruction of the child
             * classes through a base pointer.
             */
            ~BlockMatrixBase() {}


            EIGENTOOLS_DEFINE_BLOCK_MATRIX_CONSTRUCTORS(BlockMatrixBase)
    };
#undef EIGENTOOLS_PARENT_CLASS_SHORTHAND



    // ===========================================================================
    // ===========================================================================
    // ===========================================================================


    /**
     * @addtogroup BlockMatrixOperators
     * @{
     */

    /**
     * @brief 'BlockMatrixBase * Eigen::Matrix' operator
     *
     * @tparam t_MatrixType     type of raw matrix
     * @tparam t_block_rows_num number of rows in one block
     * @tparam t_block_cols_num number of columns in one block
     * @tparam t_sparsity_type  sparsity type
     * @tparam t_Derived        Eigen parameter
     *
     * @param[in] bm     block matrix
     * @param[in] matrix matrix
     *
     * @return result of multiplication
     */
    template<   typename t_MatrixType,
                int t_block_rows_num,
                int t_block_cols_num,
                MatrixSparsityType::Type t_sparsity_type,
                class t_Derived>
        EIGENTOOLS_DYNAMIC_MATRIX( typename TypeDecayed<t_MatrixType>::Type::Scalar )
        EIGENTOOLS_VISIBILITY_ATTRIBUTE
            operator* ( const BlockMatrixBase<  t_MatrixType,
                                                t_block_rows_num,
                                                t_block_cols_num,
                                                t_sparsity_type> & bm,
                        const Eigen::MatrixBase<t_Derived> & matrix)
    {
        EIGENTOOLS_DYNAMIC_MATRIX( typename TypeDecayed<t_MatrixType>::Type::Scalar ) result;
        bm.multiplyRight(result, matrix);
        return (result);
    }



    /**
     * @brief Computes 'Eigen::Matrix * BlockMatrixBase'
     *
     * @tparam t_Derived        Eigen parameter
     * @tparam t_MatrixType     type of raw matrix
     * @tparam t_block_rows_num number of rows in one block
     * @tparam t_block_cols_num number of columns in one block
     * @tparam t_sparsity_type  sparsity type
     * @tparam t_rows_in        number of rows in the matrix
     * @tparam t_cols_in        number of columns in the matrix
     *
     * @param[in] eigen_matrix  matrix
     * @param[in] bm            block matrix
     *
     * @return result of multiplication
     */
    template<   class t_Derived,
                typename t_MatrixType,
                int t_block_rows_num,
                int t_block_cols_num,
                MatrixSparsityType::Type t_sparsity_type,
                int t_rows_in,
                int t_cols_in>
        EIGENTOOLS_DYNAMIC_MATRIX( typename Eigen::DenseBase<t_Derived>::Scalar )
        EIGENTOOLS_VISIBILITY_ATTRIBUTE
            operator* ( const Eigen::MatrixBase<t_Derived> & eigen_matrix,
                        const BlockMatrixBase<  t_MatrixType,
                                                t_block_rows_num,
                                                t_block_cols_num,
                                                t_sparsity_type> & bm)
    {
        EIGENTOOLS_DYNAMIC_MATRIX( typename Eigen::DenseBase<t_Derived>::Scalar ) result;
        bm.multiplyLeft(result, eigen_matrix);
        return (result);
    }



    /**
     * @brief BlockMatrix * BlockMatrix (left * right)
     * @note 't_right_block_rows_num = t_left_block_cols_num' and therefore is
     * omitted
     *
     * @tparam t_left_MatrixType     type of raw matrix
     * @tparam t_left_block_rows_num number of rows in one block
     * @tparam t_left_block_cols_num number of columns in one block
     * @tparam t_left_sparsity_type  sparsity type
     * @tparam t_right_MatrixType    type of raw matrix
     * @tparam t_right_block_cols_num number of columns in one block
     * @tparam t_right_sparsity_type  sparsity type
     *
     * @param[in] left  block matrix
     * @param[in] right block matrix
     *
     * @return result of multiplication
     */
    template<   typename t_left_MatrixType,
                int t_left_block_rows_num,
                int t_left_block_cols_num,
                MatrixSparsityType::Type t_left_sparsity_type,
                typename t_right_MatrixType,
                int t_right_block_cols_num>
        EIGENTOOLS_DYNAMIC_MATRIX( typename TypeDecayed<t_left_MatrixType>::Type::Scalar )
        EIGENTOOLS_VISIBILITY_ATTRIBUTE
            operator* ( const BlockMatrixBase<  t_left_MatrixType,
                                                t_left_block_rows_num,
                                                t_left_block_cols_num,
                                                t_left_sparsity_type> & left,
                        const BlockMatrixBase<  t_right_MatrixType,
                                                t_left_block_cols_num,
                                                t_right_block_cols_num,
                                                MatrixSparsityType::DIAGONAL> & right)
    {
        EIGENTOOLS_DYNAMIC_MATRIX( typename TypeDecayed<t_left_MatrixType>::Type::Scalar ) result;
        right.multiplyLeft(result, left.getRaw());
        return (result);
    }


    /**
     * @brief BlockMatrix * BlockMatrix (left * right)
     * @note 't_right_block_rows_num = t_left_block_cols_num' and therefore is
     * omitted
     *
     * @tparam t_left_MatrixType     type of raw matrix
     * @tparam t_left_block_rows_num number of rows in one block
     * @tparam t_left_block_cols_num number of columns in one block
     * @tparam t_left_sparsity_type  sparsity type
     * @tparam t_right_MatrixType    type of raw matrix
     * @tparam t_right_block_cols_num number of columns in one block
     * @tparam t_right_sparsity_type  sparsity type
     *
     * @param[in] left  block matrix
     * @param[in] right block matrix
     *
     * @return result of multiplication
     */
    template<   typename t_left_MatrixType,
                int t_left_block_rows_num,
                int t_left_block_cols_num,
                MatrixSparsityType::Type t_left_sparsity_type,
                typename t_right_MatrixType,
                int t_right_block_cols_num,
                MatrixSparsityType::Type t_right_sparsity_type>
        EIGENTOOLS_DYNAMIC_MATRIX( typename TypeDecayed<t_left_MatrixType>::Type::Scalar )
        EIGENTOOLS_VISIBILITY_ATTRIBUTE
            operator* ( const BlockMatrixBase<  t_left_MatrixType,
                                                t_left_block_rows_num,
                                                t_left_block_cols_num,
                                                t_left_sparsity_type> & left,
                        const BlockMatrixBase<  t_right_MatrixType,
                                                t_left_block_cols_num,
                                                t_right_block_cols_num,
                                                t_right_sparsity_type> & right)
    {
        EIGENTOOLS_DYNAMIC_MATRIX( typename TypeDecayed<t_left_MatrixType>::Type::Scalar )   result;
        left.multiplyRight(result, right);
        return (result);
    }

    // BlockMatrixOperators
    /**
     * @}
     */
} // etools
